# Koterm

[![Pipeline Status](https://gitlab.mff.cuni.cz/teaching/nprg041/2023-24/svoboda-1040/koliandl/badges/master/pipeline.svg)](https://gitlab.mff.cuni.cz/teaching/nprg041/2023-24/svoboda-1040/koliandl/-/pipelines)
[![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)]()

Koterm is a C++ library for creating static user interfaces (UI) in terminal environments.

## Features

- Simple to use
- Multiplatform support (Linux, Windows)
- Mouse support
- Primary use is for non-changing UI

## Installation

Koterm can be installed using CMake.

```cmake
include(FetchContent)
find_package(Git REQUIRED)

FetchContent_Declare(
    koterm_repo
    GIT_REPOSITORY git@gitlab.mff.cuni.cz:teaching/nprg041/2023-24/svoboda-1040/koliandl.git
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
)
FetchContent_MakeAvailable(koterm_repo)

add_subdirectory(${koterm_repo_SOURCE_DIR}/project)

# add_executable(my_app ...)
# target_compile_features(my_app PRIVATE cxx_std_20)
# target_link_libraries(my_app PRIVATE koterm)
```

### Requirements

- C++20
- CMake
- Git

## Elements and components

### Elements

- Text [example](examples/elements/text/main.cpp)
- Canvas [example](examples/elements/canvas/main.cpp)
- Button [example](examples/elements/button/main.cpp)
- Checkbox [example](examples/elements/checkbox/main.cpp)
- Toggle [example](examples/elements/toggle/main.cpp)
- Input [example](examples/elements/input/main.cpp)
- Slider [example](examples/elements/slider/main.cpp)
- Radiobox [example](examples/elements/radiobox/main.cpp)
- VBox, HBox [example](examples/elements/boxes/main.cpp)

### Components

- Text
- Border
- TextDecoration

### Main loop

The `DomManager` class is responsible for managing the Document Object Model (DOM) of the user interface in Koterm. It handles events, focus management, rendering, and resizing of DOM elements.

1. **Handle Events**: The `DomManager` class handles various events, allowing interactive UI components to respond to user input.
2. **Render**: The DOM is rendered to the terminal, ensuring that changes to UI elements are reflected on the screen.
   1. **Update Requirements**: If an element was changed, the `update_requirements` method is called to update its requirements.
   2. **Element Rendering**: If the width or height of an element is 0, the element is not rendered on the screen.
   3. **Layout Update**: If an element is a DynamicLayout and requires a layout update, the `update_layout` method is called.
   4. **Prepare Buffer**: The `prepare_buffer` method is called to prepare the buffer for rendering.
3. **Buffer Flushing**: The buffer is flushed into stdout for display on the terminal.

### Focus system

- **Automatic Focus Queue**: Each element created is automatically added to the focus queue.
- **Focus Management**: Losing focus shifts focus to the previous element. Elements can also be focused by clicking or using keyboard shortcuts (PAGE_DOWN to select previous, PAGE_UP to select next).
- **Ordering of Elements**: Pay attention to the order in which elements are created to ensure the desired focus behavior.

### Inputs

Each input element in Koterm provides methods for handling user input events.

- **Event Handling**: Input elements offer methods like `on_change`, which returns a signal to connect delegates when events occur.
- **Event Blocking**: Events can be prevented from being emitted using the `block` method and re-enabled using `unblock`.

### Custom Elements

Custom elements in Koterm must inherit from the `Element` base class. While `calculate_requirements` is mandatory, other methods are optional and can be implemented based on the specific requirements of the custom element.

#### Required Method

- `void calculate_requirements()`: Method to calculate the requirements of the custom element.

#### Optional Methods

Custom elements can optionally implement the following methods to handle various events and interactions:

- `bool handle_event(const event::Event& event)`: Handles an event for the element.
- `bool handle_resize(const Dimensions& dimensions)`: Handles a terminal resize event.
- `bool handle_change_box(const BoundingBox& box)`: Handles the change of bounding box for the element.
- `bool handle_mouse_move(const event::MouseEvent& event)`: Handles mouse movement events for the element.
- `bool handle_mouse_click(const event::MouseEvent& event)`: Handles mouse click events for the element.
- `bool handle_mouse_scroll(const event::MouseEvent& event)`: Handles mouse scroll events for the element.
- `bool handle_input(const event::CharacterEvent& event)`: Handles input events for the element.
- `bool handle_cursor(const point_t& cursor)`: Handles response to the request for the cursor position.
- `bool handle_key(event::KeyCode key)`: Handles key events for the element.
- `bool handle_special_input(Ascii code)`: Handles special input events for the element.
- `bool has_child(element_ref element) const`: Checks if the element has a child.
- `void prepare_buffer()`: Prepares the buffer for rendering.
- `void remove_child(element_ref child)`: Removes a child element from the current element.

#### Custom Layouts

For custom layouts, inherit from the `DynamicLayout` class or from the `Element` base class based on your specific requirements.

#### Note

To maintain consistency with the Koterm library, custom elements should have a private constructor and include static creation methods. For example:

```cpp
class CustomElement : public Element {
public:
    static std::shared_ptr<CustomElement> create(const BufferSpan& buffer, DomManager* manager);
    static std::shared_ptr<Input> create(screen::BaseScreen* screen) {
        return create(BufferSpan { screen->buffer() }, &screen->dom_manager());
    }
private:
  using Element::Element;
};
```

##### Developer documentation

- [documentation](./docs/devel.md)
