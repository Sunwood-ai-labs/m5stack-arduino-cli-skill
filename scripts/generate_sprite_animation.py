#!/usr/bin/env -S uv run
# /// script
# dependencies = [
#   "pillow",
# ]
# ///

from __future__ import annotations

import argparse
import math
from pathlib import Path

from PIL import Image


TRANSPARENT_COLOR = 0x0001


def rgb888_to_rgb565(red: int, green: int, blue: int) -> int:
    return (((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3)) & 0xFFFF


def frame_indices(frame_count: int, step: int) -> list[int]:
    return list(range(0, frame_count, step))


def smooth_mix(index: int, total: int) -> float:
    linear = index / (total + 1)
    return linear * linear * (3.0 - (2.0 * linear))


def fit_frame_to_canvas(frame: Image.Image, size: int) -> Image.Image:
    rgba = frame.convert("RGBA")
    if rgba.width == 0 or rgba.height == 0:
        return Image.new("RGBA", (size, size), (0, 0, 0, 0))

    width, height = rgba.size
    scale = min(size / width, size / height)
    fitted_width = max(1, round(width * scale))
    fitted_height = max(1, round(height * scale))
    resized = rgba.resize((fitted_width, fitted_height), Image.Resampling.NEAREST)

    canvas = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    offset_x = (size - fitted_width) // 2
    offset_y = (size - fitted_height) // 2
    canvas.alpha_composite(resized, (offset_x, offset_y))
    return canvas


def convert_rgba_frame(frame: Image.Image, alpha_threshold: int) -> list[int]:
    pixels: list[int] = []
    for y in range(frame.height):
        for x in range(frame.width):
            red, green, blue, alpha = frame.getpixel((x, y))
            if alpha < alpha_threshold:
                pixels.append(TRANSPARENT_COLOR)
            else:
                pixels.append(rgb888_to_rgb565(red, green, blue))

    return pixels


def convert_frame(frame: Image.Image, size: int, alpha_threshold: int) -> list[int]:
    return convert_rgba_frame(fit_frame_to_canvas(frame, size), alpha_threshold)


def preview_rgba_frame(frame: Image.Image) -> Image.Image:
    background = Image.new("RGBA", frame.size, (52, 78, 90, 255))
    background.alpha_composite(frame)
    return background


def preview_frame(frame: Image.Image, size: int) -> Image.Image:
    return preview_rgba_frame(fit_frame_to_canvas(frame, size))


def add_loop_blend_frames(frames: list[Image.Image], blend_count: int) -> list[Image.Image]:
    if blend_count <= 0 or len(frames) < 2:
        return frames

    loop_frames = list(frames)
    start_frame = frames[0]
    end_frame = frames[-1]
    for index in range(1, blend_count + 1):
        mix = smooth_mix(index, blend_count)
        loop_frames.append(Image.blend(end_frame, start_frame, mix))
    return loop_frames


def write_header(path: Path, width: int, height: int, frames: list[list[int]]) -> None:
    lines = [
        "#pragma once",
        "",
        "#include <stdint.h>",
        "",
        f"constexpr int kCatFrameWidth = {width};",
        f"constexpr int kCatFrameHeight = {height};",
        f"constexpr int kCatFrameCount = {len(frames)};",
        f"constexpr uint16_t kCatTransparentColor = 0x{TRANSPARENT_COLOR:04X};",
        "",
        "const uint16_t kCatAnimationPixels[kCatFrameCount][kCatFrameWidth * kCatFrameHeight] = {",
    ]

    for frame in frames:
        lines.append("    {")
        for start in range(0, len(frame), 12):
            chunk = ", ".join(f"0x{value:04X}" for value in frame[start:start + 12])
            lines.append(f"        {chunk},")
        lines.append("    },")

    lines.extend(
        [
            "};",
            "",
        ]
    )

    path.write_text("\n".join(lines), encoding="utf-8")


def save_preview_strip(path: Path, frames: list[Image.Image], size: int) -> None:
    columns = len(frames)
    sheet = Image.new("RGBA", (columns * size, size), (52, 78, 90, 255))
    for index, frame in enumerate(frames):
        sheet.alpha_composite(frame, (index * size, 0))
    path.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(path)


def save_sprite_sheet(path: Path, frames: list[Image.Image], size: int, columns: int) -> None:
    rows = math.ceil(len(frames) / columns)
    sheet = Image.new("RGBA", (columns * size, rows * size), (0, 0, 0, 0))
    for index, frame in enumerate(frames):
        x = (index % columns) * size
        y = (index // columns) * size
        sheet.alpha_composite(frame, (x, y))
    path.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(path)


def main() -> None:
    parser = argparse.ArgumentParser(description="Convert an animated WebP into RGB565 animation frames.")
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--preview", type=Path)
    parser.add_argument("--sheet", type=Path)
    parser.add_argument("--size", type=int, default=112)
    parser.add_argument("--frame-step", type=int, default=4)
    parser.add_argument("--preview-frames", type=int, default=8)
    parser.add_argument("--sheet-columns", type=int, default=8)
    parser.add_argument("--alpha-threshold", type=int, default=16)
    parser.add_argument("--loop-blend-frames", type=int, default=3)
    args = parser.parse_args()

    image = Image.open(args.input)
    total_frames = getattr(image, "n_frames", 1)
    selected_indices = frame_indices(total_frames, args.frame_step)

    fitted_frames: list[Image.Image] = []

    for index in selected_indices:
        image.seek(index)
        frame = image.copy()
        fitted_frames.append(fit_frame_to_canvas(frame, args.size))

    loop_frames = add_loop_blend_frames(fitted_frames, args.loop_blend_frames)
    converted_frames = [convert_rgba_frame(frame, args.alpha_threshold) for frame in loop_frames]
    preview_frames = [preview_rgba_frame(frame) for frame in loop_frames[:args.preview_frames]]
    sheet_frames = loop_frames

    args.output.parent.mkdir(parents=True, exist_ok=True)
    write_header(args.output, args.size, args.size, converted_frames)

    if args.preview:
        save_preview_strip(args.preview, preview_frames, args.size)
    if args.sheet:
        save_sprite_sheet(args.sheet, sheet_frames, args.size, args.sheet_columns)


if __name__ == "__main__":
    main()
