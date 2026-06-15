# image_parser.py

import fitz


def extract_images(pdf_path):

    doc = fitz.open(pdf_path)

    image_info = []

    for page_number, page in enumerate(doc, start=1):

        images = page.get_images(full=True)

        image_info.append({

            "page_number": page_number,

            "number_of_images": len(images)

        })

    doc.close()

    return image_info