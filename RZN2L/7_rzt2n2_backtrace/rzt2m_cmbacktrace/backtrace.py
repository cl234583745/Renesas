import re
import sys
import time
import os
import subprocess

# Author: jingjun Mo
readelf_tool = 'arm-none-eabi-readelf.exe'
symbol_soffix = 'symbol'

def run_command(command):
    result = subprocess.run(command, capture_output=True, text=True, shell=True, encoding='utf-8')
    return result.stdout, result.stderr, result.returncode

def file_exists(file_path):
    return os.path.exists(file_path)

def find_func(symbol_table, addr):
    try:
        for symbol in symbol_table:
            addr_start = symbol['Value']
            addr_end = addr_start + symbol['Size']
            if addr < addr_start or addr > addr_end:
                continue
            # print(f"{symbol['Name']}")
            return symbol['Name']
    except Exception as e:
        print(e)
        return None

def main():
    # 获取命令行参数
    argv_len = len(sys.argv)
    if argv_len < 2:
        print("backtrace: ./backtrace [your_elf_file] [addr...]")
        return

    # sys.argv[1] 是第一个传递给脚本的参数
    elf_name = sys.argv[1]
    print(f"elf: {elf_name}")

    if not file_exists(elf_name):
        return

    # readelf_tool 
    # 示例使用
    elf_symbol_name = f"{elf_name}.{symbol_soffix}"
    stdout, stderr, return_code = run_command(f"{readelf_tool} -s {elf_name} > {elf_symbol_name}")

    # if return_code != 0:
    #     print(stderr)
    #     return
    
    if not file_exists(elf_symbol_name):
        return

    # 正则表达式匹配每一行数据
    pattern = re.compile(
        r'(\d+):\s+(\w+)\s+(\d+)\s+(\w+)\s+(\w+)\s+(\w+)\s+(\w+)\s*(.*)'
    )

    # 打开文件并逐行读取
    entries = []
    try:
        with open(elf_symbol_name, 'r') as file:
            data = file.read()
            for match in pattern.finditer(data):
                entry = {
                    'Num': int(match.group(1)),
                    'Value': int(match.group(2), 16),
                    'Size': int(match.group(3)),
                    'Type': match.group(4),
                    'Bind': match.group(5),
                    'Vis': match.group(6),
                    'Ndx': match.group(7),
                    'Name': match.group(8).strip()
                }
                if entry['Type'] == "FUNC":
                    entries.append(entry)
    except Exception as e:
        print(e)
        return
    
    str = ""
    first = 0
    try:
        for i in range(2, argv_len):
            decimal_number = int(sys.argv[i], 16)
            # print(decimal_number)
            func_name = find_func(entries, decimal_number)
            if not func_name:
                continue
            if first == 0:
                str += f"{func_name}"
                first = 1
            else:
                str += f" --> {func_name}"
    except Exception as e:
        print(e)
    # for entry in entries:
    #     print(entry)
    print(str)

    
if __name__ == "__main__":
    main()
