#!/bin/bash

# Script to generate coverage reports for NexoEngine

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}NexoEngine Coverage Report Generator${NC}"
echo "======================================"

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ] || [ ! -d "engine" ]; then
    echo -e "${RED}Error: This script must be run from the NexoEngine root directory${NC}"
    exit 1
fi

# Check for required tools
if ! command -v lcov &> /dev/null; then
    echo -e "${RED}Error: lcov is not installed${NC}"
    echo "Install with: sudo apt-get install lcov (Ubuntu/Debian) or brew install lcov (macOS)"
    exit 1
fi

if ! command -v genhtml &> /dev/null; then
    echo -e "${RED}Error: genhtml is not installed (usually comes with lcov)${NC}"
    exit 1
fi

# Default build directory
BUILD_DIR="build"
if [ "$1" != "" ]; then
    BUILD_DIR="$1"
fi

echo -e "${YELLOW}Using build directory: $BUILD_DIR${NC}"

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Creating build directory...${NC}"
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Configure with coverage enabled
echo -e "${YELLOW}Configuring with coverage enabled...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Debug -DNEXO_COVERAGE=ON -DNEXO_BUILD_TESTS=ON

# Build the project
echo -e "${YELLOW}Building project...${NC}"
make -j$(nproc)

# Clean previous coverage data
echo -e "${YELLOW}Cleaning previous coverage data...${NC}"
find . -name "*.gcda" -delete 2>/dev/null || true

# Run tests
echo -e "${YELLOW}Running tests...${NC}"
ctest --output-on-failure || true

# Generate coverage report
echo -e "${YELLOW}Generating coverage report...${NC}"
make coverage

# Summary
echo ""
echo -e "${GREEN}Coverage report generated successfully!${NC}"
echo -e "View the report: ${BUILD_DIR}/coverage/html/index.html"
echo ""

# Try to get coverage percentage
if [ -f "coverage/coverage.filtered.info" ]; then
    COVERAGE=$(lcov --summary coverage/coverage.filtered.info 2>&1 | grep -E "lines\.\.\.\.\.\.: [0-9]+\.[0-9]+%" | sed 's/.*lines\.*: \([0-9]*\.[0-9]*%\).*/\1/')
    if [ "$COVERAGE" != "" ]; then
        echo -e "${GREEN}Overall coverage: $COVERAGE${NC}"
    fi
fi