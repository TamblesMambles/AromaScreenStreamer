FROM devkitpro/devkitppc:latest

# Set working directory
WORKDIR /app

# Copy source code
COPY . /app

# Compile the Wii U homebrew app
RUN make

# Set entrypoint to run the compiled binary
CMD ["make", "run"]
