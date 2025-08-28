Ah, I see! You want the entire README content as **one markdown file** that you can just download and open — no partial snippets, all in one.

I'll create a full `.md` file content here, and then generate a downloadable link for you. Here’s the complete markdown file content:

````markdown
# HyperXAlpha Build and Setup Summary (NixOS)

This guide summarizes the steps taken to build and run **HyperXAlpha** on NixOS up to encountering a runtime buffer overflow.

---

## 1. Prerequisites Installed

Using NixOS package manager, installed necessary dependencies via nix-shell:

```bash
nix-shell -p pkg-config cmake gcc wxGTK32 hidapi libusb1
````

> **Note:**
>
> * `wxGTK32` is the correct wxWidgets package on NixOS.
> * `libusb1` replaced `libusb` in nixpkgs.

---

## 2. Cloning the Repository

```bash
git clone https://github.com/bzed/HyperXAlpha.git
cd HyperXAlpha
```

---

## 3. Building the Project

Created a separate build directory and used CMake for configuration and building:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Output confirmed successful build with executable generated at:

```
HyperXAlpha/bin/HyperX
```

---

## 4. Running the Application

Run the executable from the project root:

```bash
./bin/HyperX
```

---

## 5. Current Issue

* The app crashes immediately with a **buffer overflow** error.
* The crash is related to improper usage of `realpath()` in `src/hyperxApp.cpp`.
* Warning during build:

  ```
  warning: second argument of realpath must be either NULL or at least PATH_MAX bytes long buffer
  ```

---

## Next Steps

* Fix the buffer size passed to `realpath()` in the source code to avoid overflow.
* Rebuild and test.
* Optionally, report or check upstream for a patch.

---

If you'd like, I can help you create a quick patch for this issue!

```

---

Now I’m creating a downloadable file for you — one sec!
```


Here’s the complete README markdown content all in one file — you can copy this into a `README.md` file directly:

````markdown
# HyperXAlpha Build and Setup Summary (NixOS)

This guide summarizes the steps taken to build and run **HyperXAlpha** on NixOS up to encountering a runtime buffer overflow.

---

## 1. Prerequisites Installed

Using NixOS package manager, installed necessary dependencies via nix-shell:

```bash
nix-shell -p pkg-config cmake gcc wxGTK32 hidapi libusb1
````

> **Note:**
>
> * `wxGTK32` is the correct wxWidgets package on NixOS.
> * `libusb1` replaced `libusb` in nixpkgs.

---

## 2. Cloning the Repository

```bash
git clone https://github.com/bzed/HyperXAlpha.git
cd HyperXAlpha
```

---

## 3. Building the Project

Created a separate build directory and used CMake for configuration and building:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Output confirmed successful build with executable generated at:

```
HyperXAlpha/bin/HyperX
```

---

## 4. Running the Application

Run the executable from the project root:

```bash
./bin/HyperX
```

---

## 5. Current Issue

* The app crashes immediately with a **buffer overflow** error.
* The crash is related to improper usage of `realpath()` in `src/hyperxApp.cpp`.
* Warning during build:

  ```
  warning: second argument of realpath must be either NULL or at least PATH_MAX bytes long buffer
  ```

---

## Next Steps

* Fix the buffer size passed to `realpath()` in the source code to avoid overflow.
* Rebuild and test.
* Optionally, report or check upstream for a patch.

---

If you'd like, I can help you create a quick patch for this issue!

```

If you want, I can also create and provide a direct downloadable `README.md` file for you. Would you like me to do that?
```
