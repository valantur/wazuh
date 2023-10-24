#!/bin/bash
# Dependencies:
#   engine-test
#   engine-diff
#   jq
#   wget
#   evtx2xml
# Arguments:
#   $1: Integration name
#   $2: Path to the input file or URL
#   $3: Path to the output file or URL

#   Check Events Diff
if [[ "$#" -ne 3 ]]; then
    echo "Usage: $0 <integration> <fileInput> <fileOutput>"
    exit 1
fi

integration=$1
fileInput=$2
fileOutput=$3


# Download a file if it is a URL
download_if_url() {
    local path=$1
    # Check if the path is a URL
    if [[ $path == http://* ]] || [[ $path == https://* ]]; then
        # Download the file and save it to a temporary file
        local tempFile=$(mktemp)
        wget -O "$tempFile" "$path"
        # Extract the file extension from the URL
        local fileExtension="${path##*.}"
        echo "$tempFile" "$fileExtension"  # Return the path to the temporary file and the file extension
    else
        # Return the original path and extract the file extension from the path
        local fileExtension="${path##*.}"
        echo "$path" "$fileExtension"
    fi
}

# Convert an EVTX file to XML if it is an EVTX file
convert_if_evtx() {
    local path=$1
    local fileExtension=$2
    # Check if the file extension is evtx
    if [[ $fileExtension == "evtx" ]]; then
        # Create a temporary file
        local tempFile=$(mktemp)
        # Convert the EVTX file to XML and save it to the temporary file
        evtx2xml "$path" > "$tempFile"
        if [[ $? -ne 0 ]]; then
            echo "Error: evtx2xml failed."
            exit 1
        fi
        echo "$tempFile"  # Return the path to the temporary file
    else
        # Return the original path
        echo "$path"
    fi
}


# Check if the dependencies are installed
for cmd in engine-test engine-diff jq wget delta evtx2xml; do
    command -v $cmd >/dev/null 2>&1 || { echo >&2 "Error: $cmd is not installed."; exit 1; }
done

# Download the files if they are URLs
echo "Checking files..."

read -r fileInput fileInputExtension <<< "$(download_if_url "$fileInput")"
read -r fileOutput fileOutputExtension <<< "$(download_if_url "$fileOutput")"

# Convert the files if they are EVTX files
echo "Converting files..."
fileInput=$(convert_if_evtx "$fileInput" "$fileInputExtension")


# Check that the files exist
if [[ ! -f "$fileInput" ]] || [[ ! -f "$fileOutput" ]]; then
    echo "Error: One or both of the specified files do not exist."
    exit 1
fi

# Execute engine-test and save the output to a temporary file
echo "Running engine-test..."
tempFile=$(mktemp)
cat "$fileInput" | engine-test run -j "$integration" > "$tempFile"

# Check if engine-test failed
if [[ $? -ne 0 ]]; then
    echo "Error: engine-test failed."
    rm "$tempFile"
    exit 1
fi

# Convert each event (each line) in the temporary file to a JSON array and save it to a new file
echo "Formatting output..."
jq -s . "$tempFile" > "formatted_output.json"
if [[ $? -ne 0 ]]; then
    echo "Error: jq failed."
    cat "$tempFile"
    rm "$tempFile"
    exit 1
fi

# Compare the two files
engine-diff "formatted_output.json" "$fileOutput"

# Elimina el archivo temporal
rm "$tempFile"
rm "formatted_output.json"