# ==========================================
# category_detector.py
# ==========================================

import pymupdf
import re


def detect_categories(pdf_path):

    # Open PDF
    doc = pymupdf.open(pdf_path)

    # ======================================
    # Initialize Counters
    # ======================================

    heading_count = 0

    bullet_count = 0

    paragraph_count = 0

    image_count = 0

    figure_names = []

    table_names = []


    # Bullet symbols supported
    bullet_symbols = (

        "•",
        "·",
        "",
        "➢",
        "-",
        "*",
        "o"

    )


    # ======================================
    # Process each page
    # ======================================

    for page in doc:

        # Count images
        images = page.get_images(full=True)

        image_count += len(images)


        # Extract page blocks
        blocks = page.get_text("dict")["blocks"]


        # Process each block
        for block in blocks:

            # Ignore image blocks
            if block["type"] != 0:
                continue


            for line in block["lines"]:

                # Combine spans into one line
                line_text = ""

                for span in line["spans"]:

                    line_text += span["text"]

                line_text = line_text.strip()

                if not line_text:
                    continue


                # ==================================
                # Maximum font size in the line
                # ==================================

                max_font_size = max(

                    span["size"]

                    for span in line["spans"]

                )


                # ==================================
                # Heading Detection
                # ==================================

                if max_font_size >= 14:

                    heading_count += 1

                    continue


                # ==================================
                # Bullet Detection
                # ==================================

                if line_text.startswith(bullet_symbols):

                    bullet_count += 1

                    continue


                # ==================================
                # Figure Caption Detection
                # ==================================

                figure_match = re.match(

                    r"^(Fig\.?|Figure)\s*\d+.*",

                    line_text,

                    re.IGNORECASE

                )

                if figure_match:

                    figure_names.append(

                        line_text

                    )

                    continue


                # ==================================
                # Table Caption Detection
                # ==================================

                table_match = re.match(

                    r"^Table\s*\d+.*",

                    line_text,

                    re.IGNORECASE

                )

                if table_match:

                    table_names.append(

                        line_text

                    )

                    continue


                # ==================================
                # Paragraph Detection
                # ==================================

                paragraph_count += 1


    # Close document
    doc.close()


    # ======================================
    # Return category dictionary
    # ======================================

    return {

        "headings": heading_count,

        "bullet_points": bullet_count,

        "paragraphs": paragraph_count,

        "images": image_count,

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