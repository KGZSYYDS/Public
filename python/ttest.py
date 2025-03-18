import os
import re
import zipfile
import pandas as pd
from tqdm import tqdm
from datetime import datetime, timedelta
import logging

# 配置日志功能
def setup_logging(log_file_path):
    """
    配置日志功能，将日志信息输出到文件和控制台。
    """
    logging.basicConfig(
        level=logging.DEBUG,  # 设置日志级别为DEBUG
        format="%(asctime)s - %(levelname)s - %(message)s",  # 日志格式
        handlers=[
            logging.FileHandler(log_file_path),  # 将日志写入指定文件
            logging.StreamHandler()  # 同时将日志输出到控制台
        ]
    )
    logging.info("日志系统已启动")  # 记录日志系统启动的信息

# 处理同一天的所有ZIP文件
def process_one_day(zip_info_list, date_str, poi_df, output_dir, zip_password):
    """
    处理同一天的所有ZIP文件，解压并合并CSV文件，按四个时间窗口统计每个p_id的人数，
    并将结果与POI信息合并后输出结果文件。
    """
    logging.info(f"开始处理日期 {date_str}，包含 {len(zip_info_list)} 个 ZIP 文件")
    all_dfs = []  # 用于存储所有CSV文件的DataFrame

    # 定义时间窗口
    time_windows = [
        ("T1", 0, 6),
        ("T2", 6, 12),
        ("T3", 12, 18),
        ("T4", 18, 24),
    ]

    # 获取脚本所在目录
    script_dir = os.path.dirname(os.path.abspath(__file__))
    # 创建临时解压目录
    temp_extract_dir = os.path.join(script_dir, "temp_extract", date_str)
    os.makedirs(temp_extract_dir, exist_ok=True)

    # 解压并加载数据
    for item in tqdm(zip_info_list, desc=f"解压 {date_str} 文件进度", unit="zip"):
        zip_path = item["zip_path"]
        category = item["category"]
        cat_dir = os.path.join(temp_extract_dir, category)
        os.makedirs(cat_dir, exist_ok=True)

        try:
            with zipfile.ZipFile(zip_path, 'r') as zf:
                zf.setpassword(zip_password.encode('utf-8'))  # 设置解压密码
                zf.extractall(path=cat_dir)  # 解压到指定目录
            logging.info(f"解压成功: {zip_path} -> {cat_dir}")
        except Exception as e:
            logging.warning(f"解压失败: {zip_path}, 错误: {e}")
            continue

        for root, _, files in os.walk(cat_dir):
            for f in files:
                if f.lower().endswith(".csv"):
                    csv_path = os.path.join(root, f)
                    try:
                        df = pd.read_csv(
                            csv_path,
                            encoding='utf-8',
                            dtype=str,
                            on_bad_lines='skip'
                        )
                        all_dfs.append(df)
                        logging.info(f"读取成功: {csv_path} (行数={len(df)})")
                    except Exception as e2:
                        logging.warning(f"读取失败: {csv_path}, 错误: {e2}")

    if not all_dfs:
        logging.warning(f"日期 {date_str} 无有效数据")
        return

    # 合并所有数据
    merged_df = pd.concat(all_dfs, ignore_index=True)
    logging.info(f"合并完成: 日期 {date_str} 数据总行数={len(merged_df)}")

    # 筛选POI属于深圳市的数据
    valid_pois = poi_df.query("poi所属省份 == '广东省' and poi所属城市 == '深圳市'")
    valid_pids = valid_pois["p_id"].astype(str).tolist()
    filtered_df = merged_df[merged_df["p_id"].isin(valid_pids)]

    logging.info(f"筛选完成: 日期 {date_str} 筛选后的数据总行数={len(filtered_df)}")

    # 时间窗口统计
    tqdm_stats = tqdm(total=len(filtered_df), desc=f"统计 {date_str} 时间窗口", unit="记录")
    agg_dict = {}

    folder_date = datetime.strptime(date_str, "%Y-%m-%d").date()

    for idx, row in filtered_df.iterrows():
        try:
            p_id = row["p_id"]
            user_id = row["脱敏ID"]
            latitude = row["纬度"]
            longitude = row["经度"]
            start_dt = datetime.strptime(row["开始时间"].strip(), "%Y-%m-%d %H:%M:%S")
            end_dt = datetime.strptime(row["结束时间"].strip(), "%Y-%m-%d %H:%M:%S")
        except Exception as e:
            logging.debug(f"跳过记录: 时间解析失败, 行号: {idx}, 错误: {e}")
            tqdm_stats.update(1)
            continue

        # 初始化POI的时间窗口记录
        if p_id not in agg_dict:
            agg_dict[p_id] = {
                "经度": longitude,
                "纬度": latitude,
                "T1": set(),
                "T2": set(),
                "T3": set(),
                "T4": set(),
            }

        # 时间窗口统计
        for tw_label, win_start_hour, win_end_hour in time_windows:
            day_start = start_dt.replace(hour=0, minute=0, second=0)
            win_start = day_start.replace(hour=win_start_hour, minute=0, second=0)
            win_end = day_start.replace(hour=win_end_hour - 1, minute=59, second=59)

            # 检查时间交集
            if not (end_dt < win_start or start_dt > win_end):
                agg_dict[p_id][tw_label].add(user_id)

        # 跨天记录直接计入T4
        if end_dt.date() > folder_date:
            agg_dict[p_id]["T4"].add(user_id)

        tqdm_stats.update(1)

    tqdm_stats.close()

    # 汇总统计结果
    stat_records = []
    for p_id, time_data in agg_dict.items():
        stat_records.append([
            p_id,
            time_data["经度"],
            time_data["纬度"],
            len(time_data["T1"]),
            len(time_data["T2"]),
            len(time_data["T3"]),
            len(time_data["T4"]),
        ])
    stat_df = pd.DataFrame(stat_records, columns=["p_id", "经度", "纬度", "T1", "T2", "T3", "T4"])

    # 合并POI信息
    final_df = stat_df.merge(valid_pois, how="left", on="p_id")

    # 指定输出列的顺序
    final_cols = [
        "p_id", "T1", "T2", "T3", "T4", "经度", "纬度",
        "场景名称", "品牌名称", "poi所属省份", "poi所属城市",
        "poi详细地址", "场景一级分类", "场景二级分类", "场景三级分类"
    ]

    for col in final_cols:
        if col not in final_df.columns:
            final_df[col] = None

    final_df = final_df[final_cols]

    # 输出文件
    out_filename = f"{date_str.split('-')[1]}.{date_str.split('-')[2]}.csv"
    out_path = os.path.join(output_dir, out_filename)
    final_df.to_csv(out_path, index=False, encoding="utf-8")
    logging.info(f"处理完成: 日期 {date_str} -> {out_path} (行数={len(final_df)})")


def main():
    """
    主函数：处理所有日期的ZIP文件
    """
    script_dir = os.path.dirname(os.path.abspath(__file__))
    source_dir = r"Z:\Data\深圳数据\深圳20241015数据"
    poi_csv_path = os.path.join(script_dir, "poi_info_one.csv")
    output_dir = os.path.join(script_dir, "wifi_stat")
    os.makedirs(output_dir, exist_ok=True)

    log_file_path = os.path.join(script_dir, "wifi_stat.log")
    setup_logging(log_file_path)

    zip_password = input("请输入解压密码: ")

    zip_files = [f for f in os.listdir(source_dir) if f.lower().endswith(".zip")]
    pattern = re.compile(r"^(SceneReco|WifiConnect|WifiStable)_(\d{4}-\d{1,2}-\d{1,2})_.*\.zip$", re.IGNORECASE)
    date_map = {}
    for zip_file in zip_files:
        match = pattern.match(zip_file)
        if not match:
            continue
        category = match.group(1)
        date_str = match.group(2)
        zip_path = os.path.join(source_dir, zip_file)
        if date_str not in date_map:
            date_map[date_str
