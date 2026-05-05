#!/usr/bin/env -S uv run
# /// script
# dependencies = [
#   "pillow",
# ]
# ///

from __future__ import annotations

import argparse
import re
from pathlib import Path

from PIL import Image


def parse_header(header_path: Path) -> tuple[int, int, int, int, list[list[int]]]:
    text = header_path.read_text(encoding="utf-8")

    width = int(re.search(r"constexpr int \w+FrameWidth = (\d+);", text).group(1))
    height = int(re.search(r"constexpr int \w+FrameHeight = (\d+);", text).group(1))
    frame_count = int(re.search(r"constexpr int \w+FrameCount = (\d+);", text).group(1))
    transparent = int(re.search(r"constexpr uint16_t \w+TransparentColor = 0x([0-9A-Fa-f]{4});", text).group(1), 16)

    values = [int(match, 16) for match in re.findall(r"0x([0-9A-Fa-f]{4})", text)]
    values = values[1:]

    pixels_per_frame = width * height
    expected_values = frame_count * pixels_per_frame
    if len(values) != expected_values:
        raise ValueError(
            f"Parsed {len(values)} pixel values, expected {expected_values} "
            f"({frame_count} frames x {pixels_per_frame} pixels)."
        )

    frames: list[list[int]] = []
    for start in range(0, len(values), pixels_per_frame):
        frames.append(values[start:start + pixels_per_frame])

    return width, height, frame_count, transparent, frames


def rgb565_to_rgba(value: int, transparent: int) -> tuple[int, int, int, int]:
    if value == transparent:
        return (0, 0, 0, 0)

    red = ((value >> 11) & 0x1F) * 255 // 31
    green = ((value >> 5) & 0x3F) * 255 // 63
    blue = (value & 0x1F) * 255 // 31
    return (red, green, blue, 255)


def frame_to_image(frame: list[int], width: int, height: int, transparent: int) -> Image.Image:
    image = Image.new("RGBA", (width, height))
    image.putdata([rgb565_to_rgba(value, transparent) for value in frame])
    return image


def composite_on_background(frame: Image.Image, background: tuple[int, int, int, int]) -> Image.Image:
    canvas = Image.new("RGBA", frame.size, background)
    canvas.alpha_composite(frame)
    return canvas


def parse_background(raw: str) -> tuple[int, int, int, int]:
    channels = [int(part.strip()) for part in raw.split(",")]
    if len(channels) == 3:
        return (channels[0], channels[1], channels[2], 255)
    if len(channels) == 4:
        return (channels[0], channels[1], channels[2], channels[3])
    raise ValueError("--background must be 'R,G,B' or 'R,G,B,A'")


def save_apng(
    output_path: Path,
    frames: list[Image.Image],
    duration_ms: int,
    loop: int,
) -> None:
    output_path.parent.mkdir(parents=True, exist_ok=True)
    frames[0].save(
        output_path,
        save_all=True,
        append_images=frames[1:],
        duration=duration_ms,
        loop=loop,
        disposal=2,
        optimize=False,
    )


def main() -> None:
    parser = argparse.ArgumentParser(description="Render an RGB565 animation header as an APNG.")
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--duration-ms", type=int, default=120)
    parser.add_argument("--loop", type=int, default=0)
    parser.add_argument("--background", default="52,78,90,255")
    args = parser.parse_args()

    width, height, _frame_count, transparent, frames = parse_header(args.input)
    background = parse_background(args.background)
    images = [
        composite_on_background(frame_to_image(frame, width, height, transparent), background)
        for frame in frames
    ]
    save_apng(args.output, images, args.duration_ms, args.loop)


if __name__ == "__main__":
    main()
