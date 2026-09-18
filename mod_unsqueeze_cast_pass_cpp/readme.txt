1) 把ops-nn/build/autogen目录下的mod_unsqueeze_cast_proto.cpp和mod_unsqueeze_cast_proto.h复制到mod_unsqueeze_cast_pass/proto
2) 把mod_unsqueeze_cast_pass/gen_es_api/CMakeLists.txt中的文件路径改成
    add_library(custom_op_proto SHARED
        ../proto/mod_unsqueeze_cast_proto.cpp)
3) 在mod_unsqueeze_cast_pass/src中编写mod_unsqueeze_cast_pass.cpp
4) 执行以下命令编译、安装：
mkdir build && cd build
cmake ..
make -j$(nproc)
make install
5) 删除已安装的pass：
cd build
make clean_custom_pass
6) 验证pass是否写入了GE：
export DUMP_GE_GRAPH=1
export ASCEND_SLOG_PRINT_TO_STDOUT=1
export ASCEND_GLOBAL_LOG_LEVEL=0
atc --model=你的模型.onnx --framework=5 \
    --soc_version=Ascend910B3 --output=./out --log=debug