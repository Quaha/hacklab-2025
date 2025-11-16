#!/bin/bash

# Script for uploading solution results to OBS
# Usage: ./upload_solution.sh

set -e

# Configuration
DATE=$(date +%Y-%m-%d-%H-%M-%S)
ARCHIVE_NAME="solution-${DATE}.tar.gz"
PARCELS_DIR="parcels"

# File paths
# TEAM_CONFIG_FILE expects a string with team name in it
TEAM_CONFIG_FILE="$HOME/.upload_solution_team"
# OBS_CONFIG_FILE is generated using obsutil. We expect it
# to be already created
OBS_CONFIG_FILE="$HOME/.obsutilconfig"

# Directory and file patterns
SRC_DIR="src"
INTERNAL_DIR="hackathon_internals"
INTERFACE_FILE="interface.cpp"
MAKEFILE="Makefile"
APP_FILE="app"
README_FILE="README.md"
DOCKERFILE="Dockerfile"

# OBS bucket naming
BUCKET_PREFIX="hackathon-team-"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_step() {
    echo -e "${BLUE}[STEP]${NC} $1"
}

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

# Function to check if obsutil is installed
check_obsutil() {
    print_step "Checking if required tools are installed..."
    
    if ! command -v obsutil &> /dev/null; then
        print_error "Required tool 'obsutil' is not installed on this computer"
        echo
        echo "This is a technical issue that requires administrator help."
        echo "Please contact support in the hackathon chat with this message:"
        echo "'obsutil tool is missing - need installation help'"
        exit 1
    else
        print_success "Required tools are installed"
    fi
}

# Function to check if .obsutilconfig exists
check_config() {
    print_step "Checking configuration file..."
    
    if [ ! -f "$OBS_CONFIG_FILE" ]; then
        print_error "Configuration file is missing"
        echo
        echo "This file should have been set up for you automatically."
        echo "If it's missing, please contact support in the hackathon chat with this message:"
        echo "'.obsutilconfig file is missing - need configuration help'"
        exit 1
    else
        print_success "Configuration file found"
    fi
}

# Function to load configuration
load_config() {
    print_step "Loading your team configuration..."
    
    # Check if team configuration file exists
    if [ ! -f "$TEAM_CONFIG_FILE" ]; then
        print_error "Team configuration file is missing"
        echo "Please contact support in hackathon chat with this message:"
        echo "'.upload_solution_team file is missing - need configuration help'"
        exit 1
    fi
    
    # Read the team name from the file (strip whitespace and newlines)
    TEAM=$(cat "$TEAM_CONFIG_FILE" | tr -d '[:space:]')
    
    if [ -z "$TEAM" ]; then
        print_error "Team name is not configured or file is empty"
        echo "Please contact support in hackathon chat with this message:"
        echo "'TEAM name is missing from configuration'"
        exit 1
    fi
    
    # Also check if obsutil config exists for upload credentials
    if [ ! -f "$OBS_CONFIG_FILE" ]; then
        print_error "Upload credentials file is missing"
        echo "Please contact support in hackathon chat with this message:"
        echo "'.obsutilconfig file is missing - need configuration help'"
        exit 1
    fi
    
    print_success "Configuration loaded successfully for team: $TEAM"
}

# Function to configure obsutil
configure_obsutil() {
    print_step "Setting up upload connection..."
    # obsutil is already configured by the config file
    print_success "Upload connection configured"
}

# Function to create solution archive
create_archive() {
    print_step "Preparing your solution for upload..."
    
    echo "Creating a package with all your solution files..."
    
    # Create parcels directory with team subdirectory
    TEAM_PARCELS_DIR="${PARCELS_DIR}/${TEAM}"
    mkdir -p "$TEAM_PARCELS_DIR"
    
    # Create temporary directory for packaging
    TEMP_DIR="temp_solution_${DATE}"
    mkdir -p "$TEMP_DIR"
    
    # Copy essential files
    echo "Collecting your solution files..."
    
    # Copy source files
    if [ -d "$SRC_DIR" ]; then
        cp -r "$SRC_DIR/" "$TEMP_DIR/"
        echo "✓ Source code files included"
    else
        echo "⚠ Source code directory not found (this might be normal)"
    fi
    
    if [ -d "$INTERNAL_DIR" ]; then
        cp -r "$INTERNAL_DIR/" "$TEMP_DIR/"
        echo "✓ Internal files included"
    else
        echo "⚠ Internal files directory not found (this might be normal)"
    fi
    
    # Copy build artifacts and executables
    if [ -f "$INTERFACE_FILE" ]; then
        cp "$INTERFACE_FILE" "$TEMP_DIR/"
        echo "✓ Interface file included"
    else
        echo "⚠ Interface file not found"
    fi
    
    if [ -f "$MAKEFILE" ]; then
        cp "$MAKEFILE" "$TEMP_DIR/"
        echo "✓ Build file included"
    else
        echo "⚠ Build file not found"
    fi
    
    if [ -f "$APP_FILE" ]; then
        cp "$APP_FILE" "$TEMP_DIR/"
        echo "✓ Your program included"
    else
        echo "⚠ Your program file not found"
    fi
    
    if [ -f "$README_FILE" ]; then
        cp "$README_FILE" "$TEMP_DIR/"
        echo "✓ Documentation included"
    fi
    
    if [ -f "$DOCKERFILE" ]; then
        cp "$DOCKERFILE" "$TEMP_DIR/"
        echo "✓ Docker configuration included"
    else
        echo "⚠ Docker configuration not found (this might be normal)"
    fi
    
    # Create a metadata file
    cat > "$TEMP_DIR/metadata.txt" << EOF
Solution Package Information
============================
Team: ${TEAM}
Packaged on: ${DATE}
Package Name: ${ARCHIVE_NAME}
EOF
    
    echo "✓ Package information created"
    
    # Create the archive in team parcels directory
    ARCHIVE_PATH="${TEAM_PARCELS_DIR}/${ARCHIVE_NAME}"
    tar -czf "${ARCHIVE_PATH}" -C "$TEMP_DIR" .
    
    # Clean up temporary directory
    rm -rf "$TEMP_DIR"
    
    print_success "Solution package created: ${ARCHIVE_NAME}"
    echo "Package size: $(du -h "${ARCHIVE_PATH}" | cut -f1)"
}

# Function to upload to OBS
upload_to_obs() {
    print_step "Uploading your solution to the hackathon server..."
    
    echo "This may take a few moments depending on your internet connection..."
    echo
    
    # OBS bucket path - use standard bucket naming (lowercase, no special chars except hyphen)
    BUCKET_NAME="${BUCKET_PREFIX}${TEAM}"
    OBS_PATH="obs://${BUCKET_NAME}/${ARCHIVE_NAME}"
    ARCHIVE_PATH="${PARCELS_DIR}/${TEAM}/${ARCHIVE_NAME}"
    
    echo "Uploading to: ${BUCKET_NAME}"
    echo "File: ${ARCHIVE_NAME}"
    echo
    
    # Upload the file with progress indication
    echo "📤 Starting upload..."
    if obsutil cp "${ARCHIVE_PATH}" "${OBS_PATH}"; then
        print_success "Upload completed successfully!"
        echo "Your solution has been submitted to the hackathon server."
    else
        print_error "Upload failed"
        echo
        echo "This could be due to:"
        echo "- No internet connection"
        echo "- Server is temporarily unavailable"
        echo "- Configuration issues"
        echo
        echo "Please contact support in the hackathon chat with this message:"
        echo "'Upload failed for team ${TEAM} - need technical help'"
        exit 1
    fi
}

# Function to cleanup
cleanup() {
    print_step "Cleaning up temporary files..."
    rm -f "${PARCELS_DIR}/${TEAM}/${ARCHIVE_NAME}"
    print_success "Temporary files removed"
}

# Function to cleanup temporary files
cleanup_temp() {
    if [ -d "$TEMP_DIR" ]; then
        print_info "Cleaning up temporary directory: $TEMP_DIR"
        rm -rf "$TEMP_DIR"
    fi
}

# Set trap to cleanup on exit
trap cleanup_temp EXIT

# Function to show completion message
show_completion() {
    echo
    echo "=================================================="
    echo "    UPLOAD COMPLETED SUCCESSFULLY! 🎉"
    echo "=================================================="
    echo
    echo "Your solution has been submitted to the hackathon!"
    echo "Team: ${TEAM}"
    echo "File: ${ARCHIVE_NAME}"
    echo "Time: $(date)"
    echo
    echo "=================================================="
}

# Main execution
main() {
    print_step "Starting upload process..."
    
    # Check prerequisites
    check_obsutil
    check_config
    
    # Load configuration
    load_config
    
    # Configure obsutil
    configure_obsutil
    
    # Build the solution first
    print_step "Building your solution..."
    echo "Compiling your code... This may take a minute."
    if make clean && make; then
        print_success "Build completed successfully!"
    else
        print_error "Build failed - there might be an error in your code"
        echo
        echo "Common reasons for build failure:"
        echo "- Syntax errors in your code"
        echo "- Missing files"
        echo "- Compilation issues"
        echo
        echo "Please check your code and try again."
        exit 1
    fi
    
    # Create archive
    create_archive
    
    # Upload to OBS
    upload_to_obs
    
    # Ask if user wants to keep the local archive
    echo
    echo -e "${YELLOW}Would you like to keep a copy of the package on this computer?${NC}"
    echo -e "${YELLOW}This is optional - your solution is already uploaded safely.${NC}"
    read -p "Keep local copy? (y/N): " -n 1 -r
    echo
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        cleanup
        echo "Local copy removed (your solution is safe on the server)"
    else
        print_success "Local copy preserved: ${PARCELS_DIR}/${TEAM}/${ARCHIVE_NAME}"
    fi
    
    show_completion
}

# Run main function
main "$@"
