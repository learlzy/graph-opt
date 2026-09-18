1) 把ops-nn/build/autogen目录下的mod_unsqueeze_cast_proto.cpp和mod_unsqueeze_cast_proto.h复制到mod_unsqueeze_cast_pass/proto
2) 在CMakeLists.txt中修改如下内容：
    a) 项目名： project(mod_unqueeze_cast_pass_python_es LANGUAGES CXX)
    b) proto路径：add_library(custom_op_proto SHARED ../proto/mod_unsqueeze_cast_proto.cpp)
3) 编译：
cmake -S . -B build
cmake --build build --target build_es_custom -j$(nproc)
4) 安装：
pip install --force-reinstall --upgrade --target ./build/whl_package \
  ./build/es_output/whl/es_custom-1.0.0-py3-none-any.whl
export PYTHONPATH="$PWD/build/whl_package:${PYTHONPATH:-}"
export LD_LIBRARY_PATH="$PWD/build/es_output/lib64:${LD_LIBRARY_PATH:-}"
5) 卸载：
rm -rf ./build/whl_package
unset PYTHONPATH
source ${ASCEND_HOME_PATH}/set_env.sh
6) 编写PatternFussionPass: mod_unsqueeze_cast_pass.py
7) 设置python pass路径：
export ASCEND_GE_PY_PASS_PATH=$(pwd)/mod_unsqueeze_cast_pass.py
8) 编译计算图：
atc --framework=5 --model=yolo26s.onnx \
--input_format=NCHW --input_shape="images:8,3,640,640" \
--log=error --soc_version=Ascend910B3  --output=yolo26s