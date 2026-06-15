# metadata_parser.py

import fitz  # PyMuPDF
import os


def extract_metadata(pdf_path):

    # Open PDF document
    doc = fitz.open(pdf_path)

    # Extract metadata dictionary
    metadata = doc.metadata

    output = {

        # Extract file name
        "file_name": os.path.basename(pdf_path),

        # Number of pages in PDF
        "total_pages": len(doc),

        # Author information
        "author": metadata.get("author"),

        # Title of document
        "title": metadata.get("title"),

        # PDF creator software
        "creator": metadata.get("creator"),

        # Producer
        "producer": metadata.get("producer")
    }

    doc.close()

    return output