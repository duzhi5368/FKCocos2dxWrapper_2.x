#!/bin/sh
echo "---------------------------android NDK 调试脚本-------------------------------"
echo "---[DEBUG]  路径: ../../Debug.android"
echo "---输入[1] 直接调试(程序已经在手机上运行)"
echo "---输入[2] 全部重新启动调试(程序可能未在手机上允许)"
echo " "
read -p "请输入指令[1~2],输入Q退出编译:" C
echo " "
if [ "$C" = "1" ];then 
				echo "ndk-gdb --verbose NDK_OUT=../../Debug.android"
       ndk-gdb --verbose NDK_OUT="../../Debug.android"
elif [ "$C" = "2" ];then 
				echo "ndk-gdb --verbose --start --force NDK_OUT=../../Debug.android"
        ndk-gdb --verbose --start --force NDK_OUT="../../Debug.android"
elif [ "$C" = "Q" ];then 
        echo "exit success!"
elif [ "$C" = "q" ];then 
        echo "exit success!"
else 
        echo  "what the fuck common:[$C]?" 
fi


