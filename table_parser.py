# ==========================================
# table_parser.py
# ==========================================

import pdfplumber
import re


def detect_tables(pdf_path):

    # Store information of all pages
    table_information = []


    # Open PDF
    with pdfplumber.open(pdf_path) as pdf:


        # =====================================
        # Process page by page
        # =====================================

        for page_number, page in enumerate(pdf.pages, start=1):


            # Extract tables from page
            tables = page.extract_tables()

            # Initialize page variables
            page_table_count = 0

            table_names = []

            rows = []

            columns = []


            # ==================================
            # Detect table captions
            # ==================================

            page_text = page.extract_text()

            if page_text:

                lines = page_text.split("\n")

                for line in lines:

                    line = line.strip()

                    table_match = re.match(

                        r"^Table\s*\d+.*",

                        line,

                        re.IGNORECASE

                    )

                    if table_match:

                        table_names.append(line)


            # ==================================
            # Process each detected table
            # ==================================

            for table in tables:

                page_table_count += 1

                # Number of rows
                num_rows = len(table)

                rows.append(num_rows)


                # Number of columns
                if num_rows > 0:

                    num_columns = len(table[0])

                else:

                    num_columns = 0

                columns.append(num_columns)


            # ==================================
            # Save page information
            # Only if tables exist
            # ==================================

            if page_table_count > 0:

                table_information.append(

                    {

                        "page_number": page_number,

                        "table_count": page_table_count,

                        "rows": rows,

                        "columns": columns,

                        "table_names":

                            table_names

                            if table_names

                            else None

                    }

                )


    return table_information