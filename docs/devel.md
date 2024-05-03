# Koterm - developers docs

## Project structure

- **include/**
  - **gen/**: Generated files (e.g., `config.h`).
  - **koterm/**: Header files for Koterm library.
    - **component/**: Header files for UI components.
    - **dom/**: Header files for Document Object Model (DOM).
      - **drawing/**: Header files for drawing elements.
      - **input/**: Header files for input elements.
      - **layout/**: Header files for layout management.
    - **event/**: Header files for event handling.
    - **screen/**: Header files for screen management.
      - **pallete/**: Header files for color palettes.
    - **terminal/**: Header files for terminal manipulation.
    - **util/**: Header files for utility functions and classes.

## Code Formatting

We use clang-format for code formatting to ensure consistency throughout the codebase. The configuration file (clang-format) is located in the root directory of the project. Before committing changes, please format your code using clang-format.

## Code Quality Checks

We use clang-tidy for static code analysis to identify potential issues and enforce coding standards. The configuration file (clang-tidy) is located in the root directory of the project. Before submitting a pull request, please run clang-tidy to check your code.

## Event handling

Event handling is essential for creating interactive UIs in Koterm. The library employs a multithreaded architecture with two main threads: one for the UI and another for parsing input and sending events to the UI.

## Terminal manipulation

Koterm relies on ANSI Escape Sequences for terminal manipulation, including rendering colors, styles, and capturing user input. While ANSI Escape Sequences are widely supported across modern terminal emulators, it's essential to note that not all terminals fully support them. However, Koterm is designed with a focus on compatibility with terminals that do support ANSI Escape Sequences.
One notable exception is the default Windows cmd.exe terminal, which requires special configuration using the Windows API for full ANSI Escape Sequence support. Koterm provides compatibility with this terminal environment through the use of the appropriate Windows API calls, ensuring a seamless user experience on Windows platforms.

## Testing

For testing purposes, Koterm utilizes doctest, a lightweight unit testing header library.
