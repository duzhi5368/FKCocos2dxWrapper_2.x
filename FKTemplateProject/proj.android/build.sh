#!/bin/sh
echo "---------------------------android NDK 编译脚本-------------------------------"
echo "---[DEBUG]  生成路径: ../../Debug.android"
echo "---输入[1] 生成调试版"
echo "---输入[2] 重新生成调试版"
echo " "
echo "---[RELEASE]生成路径: ../../Release.android"
echo "---输入[3] 生成发行版"
echo "---输入[4] 重新生成发行版"
echo " "
read -p "请输入指令[1~4],输入Q退出编译:" C
echo " "
if [ "$C" = "1" ];then 
				echo "ndk-build NDK_DEBUG=1 NDK_OUT=../../Debug.android"
        ndk-build NDK_DEBUG=1 NDK_OUT="../../Debug.android"
elif [ "$C" = "2" ];then 
				echo "ndk-build  clean all NDK_DEBUG=1 NDK_OUT=../../Debug.android"
        ndk-build  clean all NDK_DEBUG=1 NDK_OUT="../../Debug.android"
elif [ "$C" = "3" ];then 
				echo "ndk-build NDK_DEBUG=0 NDK_OUT=../../Release.android"
        ndk-build NDK_DEBUG=0 NDK_OUT="../../Release.android"
elif [ "$C" = "4" ];then 
        ndk-build  clean all NDK_DEBUG=0 NDK_OUT="../../Release.android" 
        echo "ndk-build clean all NDK_DEBUG=0 NDK_OUT=../../Release.android"
elif [ "$C" = "Q" ];then 
        echo "exit success!"
elif [ "$C" = "q" ];then 
        echo "exit success!"
else 
        echo  "what the fuck common:[$C]?" 
fi