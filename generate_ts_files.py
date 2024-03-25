import subprocess
import os
from pathlib import Path
import os,sys





def list_files(base_dir, cpp_List):
    if os.path.isdir(base_dir):
        for filename in os.listdir(base_dir):
            cur_path=os.path.join(base_dir, filename)
            if os.path.isdir(cur_path):
                list_files(cur_path, cpp_List)
            elif os.path.isfile(cur_path) and cur_path.endswith(".cpp"):
                cpp_List.append(cur_path)

        
def generate_ts_file_from_path(gui_cpp_dir):
    gui_cpp_List=[]
    list_files(gui_cpp_dir, gui_cpp_List)
    dst_st_File = 'langs\zh.ts'
    # lupdate_exe is in QT bin dir consistent with qmake
    lupdate_exe = 'lupdate.exe'
    exe_cmd_list = [lupdate_exe, ]
    exe_cmd_list.extend(gui_cpp_List)
    print(exe_cmd_list)
    exe_cmd_list.extend([ '-ts', dst_st_File, ])
    print(exe_cmd_list)
    subprocess.run(exe_cmd_list)

generate_ts_file_from_path('GUI')