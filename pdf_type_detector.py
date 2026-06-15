# pdf_type_detector.py

import fitz


def detect_pdf_type(pdf_path):

    doc = fitz.open(pdf_path)

    total_pages = len(doc)

    text_pages = 0

    # Check every page
    for page in doc:

        # Extract text
        text = page.get_text()

        # If page contains text
        if text.strip():
            text_pages += 1

    doc.close()

    # Classification
    if text_pages == total_pages:
        return "Digital PDF"

    elif text_pages == 0:
        return "Scanned PDF"

    else:
        return "Mixed PDF"