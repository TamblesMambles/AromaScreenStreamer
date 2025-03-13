FROM devkitpro/devkitppc:latest

# Install WUT (Wii U Toolchain)
RUN yes | dkp-pacman -S wiou-dev

# Set working directory
WORKDIR /app

# Copy source code
COPY . /app

# Compile the Wii U homebrew app
RUN make

# Set entrypoint to run the compiled binary
CMD ["make", "run"]
