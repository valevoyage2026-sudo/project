# ==========================================
# page_parser.py
# ==========================================

import pymupdf
import re


def parse_pages(pdf_path):

    # Open PDF
    doc = pymupdf.open(pdf_path)

    page_information = []


    # =====================================
    # Process every page
    # =====================================

    for page_number, page in enumerate(doc, start=1):

        # Extract page text
        text = page.get_text()

        # Count images
        images = page.get_images(full=True)

        image_count = len(images)


        # Initialize counters
        heading_count = 0
        bullet_count = 0
        paragraph_count = 0

        figure_names = []

        table_names = []


        # Extract layout dictionary
        blocks = page.get_text("dict")["blocks"]


        # Process each block
        for block in blocks:

            # Process only text blocks
            if block["type"] != 0:
                continue


            for line in block["lines"]:

                line_text = ""


                # Merge spans into one line
                for span in line["spans"]:

                    line_text += span["text"]


                line_text = line_text.strip()

                if not line_text:
                    continue


                # =================================
                # Heading Detection
                # =================================

                max_font_size = max(
                    span["size"]
                    for span in line["spans"]
                )

                if max_font_size >= 14:

                    heading_count += 1

                    continue


                # =================================
                # Bullet Detection
                # =================================

                bullet_symbols = (
                    "•",
                    "·",
                    "",
                    "➢",
                    "-",
                    "*",
                    "o"
                )

                if line_text.startswith(bullet_symbols):

                    bullet_count += 1

                    continue


                # =================================
                # Figure Caption Detection
                # =================================

                figure_match = re.match(

                    r"^(Fig\.?|Figure)\s*\d+.*",

                    line_text,

                    re.IGNORECASE

                )

                if figure_match:

                    figure_names.append(line_text)

                    continue


                # =================================
                # Table Caption Detection
                # =================================

                table_match = re.match(

                    r"^Table\s*\d+.*",

                    line_text,

                    re.IGNORECASE

                )

                if table_match:

                    table_names.append(line_text)

                    continue


                # =================================
                # Paragraph Detection
                # =================================

                paragraph_count += 1


        # =====================================
        # Store Page Information
        # =====================================

        page_information.append(

            {

                "page_number": page_number,

                "has_text": bool(text.strip()),

                "image_count": image_count,

                "heading_count": heading_count,

                "bullet_count": bullet_count,

                "paragraph_count": paragraph_count,

                "figure_names": (
                    figure_names
                    if figure_names
                    else None
                ),

                "table_names": (
                    table_names
                    if table_names
                    else None
                )

            }

        )


    doc.close()

    return page_information