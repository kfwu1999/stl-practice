#!/bin/bash

# Define the folders to format
FOLDERS=("../include" "../test")

# Loop through each folder
for folder in "${FOLDERS[@]}"; do
    if [ -d "$folder" ]; then
        echo "Formatting files in '$folder'..."
        # Find and format all files with .h, .hpp, .cpp, or .cc extensions
        find "$folder" -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.cpp" -o -name "*.cc" \) | while read -r file; do
            echo "Formatting $file"
            clang-format -i "$file"
        done
    else
        echo "Warning: Directory '$folder' does not exist. Skipping..."
    fi
done

echo "Formatting completed for all specified folders."

