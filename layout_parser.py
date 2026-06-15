# layout_parser.py

import fitz


def extract_layout(pdf_path):

    doc = fitz.open(pdf_path)

    layout_data = []

    for page_number, page in enumerate(doc, start=1):

        # Extract blocks
        blocks = page.get_text("dict")["blocks"]

        text_blocks = 0
        image_blocks = 0

        for block in blocks:

            # type = 0 → text
            if block["type"] == 0:
                text_blocks += 1

            # type = 1 → image
            elif block["type"] == 1:
                image_blocks += 1

        layout_data.append({

            "page_number": page_number,

            "text_blocks": text_blocks,

            "image_blocks": image_blocks

        })

    doc.close()

    return layout_data