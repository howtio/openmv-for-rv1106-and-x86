# minimv

一个正在进行中的 OpenMV 移植实验项目，当前目标是先把 OpenMV 的图像算法核心从原始固件/板级环境中拆出来，在 Linux 用户态跑通，再逐步迁移到 RV1106。

当前这版已经打通了两条最小链路：

1. **宿主机 C 测试链路**  
   `test.jpg -> image_t -> imlib_find_circles() -> 输出圆检测结果`

2. **宿主机 Python 最小兼容链路**  
   `import image -> image.Image("test.jpg") -> img.find_circles(...) -> print(...)`

这意味着项目目前已经具备：

- 从 OpenMV 中拆出部分 `imlib/common` 算法核心
- 用 Linux/标准 C 环境替代板级依赖
- 用最小 Python 扩展模块模拟 `import image` 的 OpenMV 风格调用
- 为后续 RV1106 交叉编译和摄像头接入做准备

---

## 当前状态

当前仓库不是完整 OpenMV 固件移植，而是 **OpenMV 算法核的最小可运行子集**。

目前已完成：

- 裁出 `imlib/common` 的最小依赖集合
- 通过兼容头文件屏蔽部分 MCU / CMSIS / MicroPython 依赖
- 使用 `stb_image` 从 JPG 文件加载灰度图
- 手动构造 `image_t`
- 直接调用 `imlib_find_circles()`
- 提供最小 Python 模块 `image`
- 支持以下 Python 用法：

```python
import image

img = image.Image("test.jpg")
circles = img.find_circles(
    threshold=4000,
    x_margin=15,
    y_margin=15,
    r_margin=15,
    r_min=15,
    r_max=80,
    r_step=2,
)
print(circles)
```

当前 `find_circles()` 返回值是最小版结果：

```python
[(x, y, r), ...]
```

还不是 OpenMV 官方完整的 `circle` 对象。

---

## 目录说明

```text
.
├── common/             # 裁出来的公共基础设施
├── imlib/              # OpenMV 图像算法核心（当前主要验证 find_circles）
├── modules/
│   └── py_image.c      # 最小 Python 扩展模块：import image
├── py/                 # 兼容用最小 MicroPython 头文件替身
├── build/              # 本地构建目录
├── main.c              # 当前 C 侧最小测试入口
├── stb_image.h         # JPG 读取
└── CMakeLists.txt      # 构建脚本
```

---

## 构建

### 1. 构建 C 测试程序

```bash
mkdir -p build
cmake -S . -B build
cmake --build build -j
```

生成：

```bash
build/test_run
```

运行：

```bash
cd build
./test_run
```

---

### 2. 构建 Python 扩展模块

确保系统安装了 Python 开发头文件，例如：

```bash
sudo apt install python3-dev
```

重新配置并编译：

```bash
cmake -S . -B build
cmake --build build -j
```

生成：

```bash
build/image.so
```

或带 ABI 后缀的 `.so` 文件。

---

## Python 示例

建议把测试图放在脚本同目录，并使用绝对路径或脚本相对路径。

示例 `test.py`：

```python
from pathlib import Path
import image

BASE = Path(__file__).resolve().parent
IMG_PATH = BASE / "test.jpg"

img = image.Image(str(IMG_PATH))
print("size =", img.width(), img.height())

circles = img.find_circles(
    threshold=4000,
    x_margin=15,
    y_margin=15,
    r_margin=15,
    r_min=15,
    r_max=80,
    r_step=2,
)

print("circles =", circles)

for i, c in enumerate(circles, 1):
    x, y, r = c
    print(f"[{i}] x={x}, y={y}, r={r}")
```

运行：

```bash
PYTHONPATH=build python3 test.py
```

---

## 已知限制

当前版本仍然是实验态，限制包括：

- 只验证了部分 `imlib` 算法路径
- 当前重点是 `find_circles`
- Python 层仅实现了最小 `import image`
- 暂未实现 `sensor` 模块
- 暂未实现 OpenMV 官方完整 `circle` 对象接口
- 暂未接入真实摄像头/V4L2
- 暂未接入 RV1106 ISP / MIPI / 显示链路
- 还不是完整 OpenMV 固件运行环境

---

## 下一步计划

推荐按下面顺序推进：

### 第一阶段：保持用户态最小闭环

- [x] `test.jpg -> imlib_find_circles()`
- [x] `import image -> Image("jpg") -> find_circles()`
- [ ] 返回更接近 OpenMV 的 `circle` 对象
- [ ] 补更多常用 `image` 方法

### 第二阶段：迁移到 RV1106

- [ ] 交叉编译当前最小工程到 RV1106
- [ ] 在板端跑通离线 JPG 检测
- [ ] 处理工具链 / ABI / Python 运行环境

### 第三阶段：接入真实图像源

- [ ] 用 V4L2/Rockchip 路径替换 JPG 文件输入
- [ ] 将 camera buffer 包装成 `image_t`
- [ ] 保持 `img.find_circles(...)` 调用方式尽量不变

### 第四阶段：向 OpenMV 兼容接口靠近

- [ ] 补 `sensor` 模块
- [ ] 补更多 `image.Image` 方法
- [ ] 提升脚本层兼容性

---

## 设计思路

当前项目采用的是：

**先把 OpenMV 算法核从板级和固件环境中剥离出来，在 Linux 用户态跑通，再逐步往 RV1106 真机回填硬件接口层。**

这样做的好处是：

- 先验证算法和接口，不被硬件驱动卡死
- 更容易定位是算法问题、兼容问题还是板级问题
- 有利于逐层替换为 RV1106 的真实输入输出链路

---

## 备注

这个仓库目前更适合被理解为：

> 一个面向 RV1106 的 OpenMV 最小移植实验场。

不是完整成品，但已经具备继续向板端迁移的基础。
