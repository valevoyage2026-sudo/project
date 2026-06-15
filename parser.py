# ==========================================
# parser.py
#
# Main Controller for PDF Parser
# ==========================================


# ==========================================
# Import Custom Modules
# ==========================================

from metadata_parser import extract_metadata
from pdf_type_detector import detect_pdf_type
from page_parser import parse_pages
from layout_parser import extract_layout
from image_parser import extract_images
from category_detector import detect_categories
from table_parser import detect_tables


# ==========================================
# Import Built-in Libraries
# ==========================================

import json
from pathlib import Path


# ==========================================
# PDF Path
# ==========================================

# Current directory of parser.py
current_directory = Path(__file__).parent

# Input PDF path
PDF_PATH = current_directory / "input_pdfs" / "EXP (7).pdf"


# ==========================================
# Function to Generate Unique Output Filename
# ==========================================

def get_output_filename(output_folder):

    base_name = "parsed_pdf"

    extension = ".json"

    output_file = output_folder / f"{base_name}{extension}"

    counter = 1

    # If file already exists,
    # append numbers
    while output_file.exists():

        output_file = (
            output_folder
            / f"{base_name}_{counter}{extension}"
        )

        counter += 1

    return output_file


# ==========================================
# Main Function
# ==========================================

def main():

    print("\nStarting PDF Parsing...\n")


    # =====================================
    # Metadata Extraction
    # =====================================

    metadata = extract_metadata(PDF_PATH)


    # =====================================
    # PDF Type Detection
    # =====================================

    pdf_type = detect_pdf_type(PDF_PATH)


    # =====================================
    # Page Information
    # =====================================

    pages = parse_pages(PDF_PATH)


    # =====================================
    # Layout Information
    # =====================================

    layout = extract_layout(PDF_PATH)


    # =====================================
    # Image Information
    # =====================================

    images = extract_images(PDF_PATH)


    # =====================================
    # Table Information
    # =====================================

    tables = detect_tables(PDF_PATH)


    # =====================================
    # Category Information
    # =====================================

    categories = detect_categories(PDF_PATH)


    # =====================================
    # Combine Everything
    # =====================================

    output = {

        "metadata": metadata,

        "pdf_type": pdf_type,

        "page_information": pages,

        "layout_information": layout,

        "image_information": images,

        "table_information": tables,

        "categories": categories

    }


    # =====================================
    # Create Output Folder Automatically
    # =====================================

    output_directory = current_directory / "output"

    output_directory.mkdir(
        exist_ok=True
    )


    # =====================================
    # Generate Output Filename
    # =====================================

    output_file = get_output_filename(
        output_directory
    )


    # =====================================
    # Save JSON File
    # =====================================

    with open(
            output_file,
            "w",
            encoding="utf-8"
    ) as file:

        json.dump(
            output,
            file,
            indent=4,
            ensure_ascii=False
        )


    # =====================================
    # Success Message
    # =====================================

    print("Parsing Completed Successfully")

    print("\nOutput saved at:\n")

    print(output_file)


# ==========================================
# Driver Code
# ==========================================

if __name__ == "__main__":

    main()