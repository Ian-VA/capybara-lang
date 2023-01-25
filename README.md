  <img src="https://user-images.githubusercontent.com/53247327/201985947-b7f4dd84-e05a-4965-b4ac-fc88ace0838f.png" width="200"> 

# capybara-lang 

![image](https://img.shields.io/badge/contributors-1-yellow) ![image](https://img.shields.io/badge/commits-171-yellow) ![image](https://img.shields.io/badge/status-functional-brightgreen)

### :heavy_exclamation_mark:	As of November 15th, 2022, Capybara has achieved beta status :heavy_exclamation_mark:	

### About Capybara

Capybara is a dynamically typed, semi-whitespace sensitive programming language, taking the best parts from Lua, Rust, and Python to create a readable, comprehensive and simple programming language.

### Why Capybara?

Capybara's main focus is to provide a compromise between high level and low level programming languages: while high level languages like Python have wonderful readability and power, they lack critical components that low level programmers love using, such as:

* Memory management / Pointers
* Portability
* Speed

At the same time, low level languages don't provide the myriad of comforts that high level languages like Python provide, such as:

* Readable syntax
* Dynamic typing
* Flexibility

Capybara is meant to provide the best of both worlds, allowing both high level and low level programmers to transfer seamlessly between the two. To accomplish this, Capybara provides:

* Readability
* Power and Speed
* Dynamic typing
* Portability
* Memory Management (Don't worry, its not required)
* Compatibility with new technology
* and a lot more!

Here's an example of some code in Capybara:

```python
add(int x1, ptr int x2): # all function arguments must be statically typed
  x2++ # capybara allows for pointer arithmetic
  return x1 + *x2 # pointers are still dereferenced like normal
end

main():
  local x1 = 3 # variables can be inferred
  ptr x2 = 5 # unlike other languages, you declare a pointer with the 'ptr' keyword
  add(x1, x2)
end
```

### Where do I get Capybara?

:warning: Capybara is currently too early in its development for it to be viable in a production setting. Capybara should only be meant for hobbyist use until it has been decreed stable enough to warrant otherwise. :warning:

Currently, Capybara can't be built using cmake. Cloning this repository is a viable way to experiment with the early version of Capybara.

### How do I contribute?

We need testers! Please make an Issue if you come across an error or a PR if you know how to fix it. Capybara is still in early development.

### How do I use capybara?

Capybara operates via a console. To make a capybara file, write a piece of capybara-syntax-compliant code with the file extension ".cb". Then, in the capybara console, type "compile (your_file_name)" without the .cb file extension and capybara will automatically find the file and compile it. Then, to run the file, type "run". The run command runs the previously compiled file.
