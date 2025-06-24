#!/usr/bin/env python3
from PIL import Image
import os
import sys

def split_image_into_tiles(image_path, output_dir="tilesmodificados", tile_size=32):
    """
    Splits an image into 32x32 tiles, reading from right to left.
    
    Args:
        image_path (str): Path to the input image
        output_dir (str): Directory to save the output tiles
        tile_size (int): Size of the tiles (both width and height)
    
    Returns:
        int: Number of tiles created
    """
    # Check if image exists
    if not os.path.exists(image_path):
        print(f"Error: Image file '{image_path}' not found")
        return 0
    
    # Create output directory if it doesn't exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"Created output directory: {output_dir}")
    
    try:
        # Open the image
        img = Image.open(image_path)
        width, height = img.size
        
        # Calculate how many tiles we can extract
        num_tiles_x = width // tile_size
        num_tiles_y = height // tile_size
        
        if num_tiles_x == 0 or num_tiles_y == 0:
            print(f"Error: Image is too small to create {tile_size}x{tile_size} tiles")
            return 0
        
        # Get the base filename without extension
        base_filename = os.path.splitext(os.path.basename(image_path))[0]
        
        # Counter for tiles
        tile_count = 0
        
        # Process tiles from right to left, top to bottom
        for y in range(num_tiles_y):
            for x in range(num_tiles_x - 1, -1, -1):  # Right to left
                # Calculate the tile coordinates
                left = x * tile_size
                upper = y * tile_size
                right = left + tile_size
                lower = upper + tile_size
                
                # Extract the tile
                tile = img.crop((left, upper, right, lower))
                
                # Save the tile
                tile_path = os.path.join(output_dir, f"{base_filename}_tile_{tile_count}.png")
                tile.save(tile_path)
                tile_count += 1
                
        print(f"Successfully created {tile_count} tiles in '{output_dir}'")
        return tile_count
        
    except Exception as e:
        print(f"Error processing image: {str(e)}")
        return 0

def main():
    # Check if an image path was provided
    if len(sys.argv) < 2:
        print("Usage: python tile_splitter.py <image_path> [output_directory]")
        return
    
    image_path = sys.argv[1]
    
    # Get optional output directory
    output_dir = "tilesmodificados"
    if len(sys.argv) >= 3:
        output_dir = sys.argv[2]
    
    # Process the image
    split_image_into_tiles(image_path, output_dir)

if __name__ == "__main__":
    main()
