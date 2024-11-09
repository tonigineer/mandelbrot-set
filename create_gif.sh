#!/usr/bin/env bash

if ! command -v ./bin/Mandelbrot &>/dev/null; then
    echo "Error: Mandelbrot not found. Make sure it's in the built and within the current directory."
    exit 1
fi

# Default parameters
image_width=854
image_height=480
n_max_iter=32000
magn_gain=1.25

# Predefined regions with its target magnification
regions=(
    "+0.4382046950 +0.3420529940 2000"
    "+0.3823088690 -0.3895870170 13000"
    "+0.2889350440 -0.0123748250 4000"
    "+0.2766433120 +0.0091976760 10000"
    "+0.2353369550 -0.5152623050 1500"
    "+0.1151166340 +0.6278655140 4000"
    "-0.0283750150 +0.6946544980 1000"
    "-0.1067504450 +0.8828258480 2000000000"
    "-0.6732812290 +0.3600723860 1000"
    "-0.743643887037158704752191506114774 0.131825904205311970493132056385139 1000000000000"
    "-1.0090787470 +0.3108562000 100000"
    "-1.1682350040 -0.2462116668 100000"
    "-1.3944205750 -0.0018272120 10000"
    "-1.4034457770 +0.0000000120 100000"
    "-1.7687788290 +0.0017389240 200000000"
    "-1.9401573530 +0.0000000000 600000000"
)

function show_help() {
    echo "Usage: $0 [-h] [-r index] [-c width heigh real imag maxiter magn]"
    echo ""
    echo "Options:"
    echo "  -h, --help                                          Show this help message and exit"
    echo "  -r, --region index                                  Select a region by index from the predefined list (0-9)"
    echo "  -c, --custom width heigh real imag maxiter magn     Specify your own settings with size, center, max iter. and magn."
    echo ""
}

generate_magn_steps() {
    python -c "arr = [1]
while arr[-1] < $target_magn:
    arr.append(arr[-1]*$magn_gain)
arr[-1] = $target_magn
print('\n'.join(map(str, arr)))"
}

render_mandelbrot_images() {
    local i=1
    local magn_steps=$(generate_magn_steps)
    local magn_steps_arr=($magn_steps)

    for magn in $magn_steps; do
        printf "\r * Processing images \t\033[34m %d \033[0m of \033[34m %d \033[0m  |  Magnification  \033[35m %.1e \033[0m" "$i" "${#magn_steps_arr[@]}" "$magn"

        # Create a unique hash for each parameter set
        input_string="${image_width}${image_height}${center_real}${center_imag}${n_max_iter}${magn}"
        hash=$(echo -n "$input_string" | md5sum | awk '{print $1}')

        png_file="$rendered_dir/$hash.png"
        ppm_file="$rendered_dir/$hash.ppm"
        gif_frame_file=$(printf "$gif_frames_dir/%05d.png" "$i")

        # Render image if it doesn't already exist
        if [ ! -s "$png_file" ]; then
            ./bin/Mandelbrot \
                "$image_width" "$image_height" \
                "$center_real" "$center_imag" \
                "$n_max_iter" \
                "$magn" >"$ppm_file"

            # Convert PPM to PNG and optimize
            magick "$ppm_file" -format png "$png_file"
            optipng -o4 -i0 "$png_file" >/dev/null 2>&1
            advpng -z4 "$png_file" >/dev/null 2>&1

            rm "$ppm_file"
        fi

        cp "$png_file" "$gif_frame_file"
        i=$((i + 1))
    done
}

generate_gif() {
    mkdir -p gifs
    gif_name=$(date '+Mandelbrot-%Y%m%d-%H%M%S').gif
    echo -e "\n * Generating \t\t \033[32mgif/${gif_name}\033[0m"

    magick -delay 10 -loop 0 "$gif_frames_dir/*.png" gifs/"$gif_name"

    rm -rf $gif_frames_dir
}

show_info() {
    A_R="\033[0m"
    A_RED="\033[31;1;4m"
    A_YEL="\033[33;1;4m"
    A_GRE="\033[32;1;4m"

    echo -e "Summary - Mandbrot set gif generation"
    echo -e "   Screen size \t\t ${A_YEL}${image_width}${A_R}x${A_YEL}${image_height}${A_R}"
    echo -e "   Center \t\t (${A_GRE}${center_real}${A_R}, ${A_GRE}${center_imag}${A_R}i)"
    echo -e "   Max iterations \t ${A_RED}$n_max_iter${A_R}"
    echo -e "   Magnification \t Target ${A_RED}${target_magn}${A_R}, Gain ${A_RED}${magn_gain}${A_R}"
    echo ""
}

main() {
    script_path="$(realpath "${BASH_SOURCE[0]}")"
    script_dir="$(dirname "$script_path")"

    cd "$script_dir" || exit

    show_info
    rendered_dir="rendered_images"
    gif_frames_dir="gif_frames"

    mkdir -p "$rendered_dir"
    rm -rf "$gif_frames_dir" && mkdir -p "$gif_frames_dir"

    render_mandelbrot_images
    generate_gif
}

case $1 in
-h | --help)
    show_help
    exit 0
    ;;
-r | --region)
    index=$2

    if [[ -z "$index" || ! "$index" =~ ^[0-9]+$ || $index -ge ${#regions[@]} ]]; then
        echo "Error: Invalid index. Use -h for help."
        exit 1
    fi

    read -r center_real center_imag target_magn <<<"${regions[$index]}"
    main
    ;;
-c | --custom)
    image_width=$2
    image_height=$3
    center_real=$4
    center_imag=$5
    n_max_iter=$6
    target_magn=$7

    if [[ -z "$image_width" || -z "$image_height" || -z "$center_real" || -z "$center_imag" || -z "$n_max_iter" || -z "$target_magn" ]]; then
        echo "Error: Missing arguments for -c. Use -h for help."
        exit 1
    fi

    main
    ;;
*)
    echo "Error: Unknown option $1. Use -h for help."
    exit 1
    ;;
esac
