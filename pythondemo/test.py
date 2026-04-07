import image

IMG_PATH = "/home/howtion/minimv/pythondemo/test.jpg"

img = image.Image(str(IMG_PATH))
print("size =", img.width(), img.height())

circles = img.find_circles(
    threshold=8000,
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