# DateTime Unique Filter

A C program that reads a list of date-time values in ISO 8601 format, identifies unique values, and writes them to an output file.

## Features

- Reads ISO 8601 formatted datetime strings from an input file
- Validates datetime format (basic format validation, no semantic validation)
- Identifies and filters out duplicate entries
- Writes unique datetime values to an output file

## Compilation

Use the provided Makefile:

```bash
make all          # Compile the program
make test         # Compile and run with sample data
make clean        # Remove compiled files
make help         # Show available targets
```

## Usage

```bash
./datetime_unique <input_file> <output_file>
```

### Example

```bash
./datetime_unique sample_datetimes.txt output_unique.txt
```

## Input Format

The input file should contain one ISO 8601 datetime string per line. Supported format:
- `YYYY-MM-DDTHH:MM:SS[Z]`
- Example: `2023-01-15T10:30:00Z`

## Output

The program creates an output file containing unique datetime values, one per line, in the order they were first encountered.

