import os

from functools import partial
from os import path
from PIL import Image

ASSETS_DIR = path.dirname(__file__)
PROJECT_DIR = path.dirname(ASSETS_DIR)
BUILD_DIR = path.join(PROJECT_DIR, "build")
BUILD_ASSET_DIR = path.join(BUILD_DIR, "test-assets")


def generate_mandelbrot(width: int, height: int) -> Image.Image:
    image = Image.effect_mandelbrot(
        (width, height),
        (0, 0, width, height),
        10
    )

    if image.mode != "RGBA":
        image = image.convert("RGBA")

    return image


def generate_mip_level_sizes(base_width: int, base_height: int, n_mips: int):
    for mip_level in range(1, n_mips):
        level_width = round(max(1, base_width * 0.5 ** mip_level))
        level_height = round(max(1, base_height * 0.5 ** mip_level))

        yield level_width, level_height


def generate_mip_levels(image: Image.Image, n_mips: int):
    mip_level_sizes = generate_mip_level_sizes(image.width, image.height, n_mips)

    for level_width, level_height in mip_level_sizes:
        yield image.resize((level_width, level_height))


def make_mipmaps_test_files_name(prefix: str, width: int, height: int, mip_level: int):
    return f"{prefix}-{width}x{height}-level{mip_level}.bin"


def generate_mipmaps_test_files(name_prefix: str, width: int, height: int, n_levels: int):
    get_name = partial(make_mipmaps_test_files_name, name_prefix, width, height)
    base_image = generate_mandelbrot(width, height)
    mipmap_images = generate_mip_levels(base_image, n_levels)
    file_name = get_name(0)

    write_image_asset_file(file_name, base_image)

    for mip_level, mip_image in enumerate(mipmap_images, 1):
        file_name = get_name(mip_level)
        write_image_asset_file(file_name, mip_image)


def create_folders():
    os.makedirs(BUILD_ASSET_DIR, exist_ok=True)


def write_image_asset_file(file_name: str, image: Image.Image):
    full_path = path.join(BUILD_ASSET_DIR, file_name)

    if not path.exists(full_path):
        print(f"Writing image asset data {full_path}...")

        with open(full_path, "wb") as f:
            f.write(image.tobytes())
    else:
        print(f"Skipping existing data file {full_path}.")


def main():
    create_folders()
    generate_mipmaps_test_files("image-mipmaps", 256, 256, 5)
    generate_mipmaps_test_files("image-no-mipmaps", 256, 100, 1)


if __name__ == "__main__":
    main()
