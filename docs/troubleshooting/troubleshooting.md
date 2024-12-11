\page troubleshooting Troubleshooting

# Build Troubleshooting

## Table of Contents

- [Common Issues](#common-issues)
  - [Couldn't create symlink](#couldnt-create-symlink)
  - [CPackDeb: file utility is not available](#cpackdeb-file-utility-is-not-available)
- [Still having issues? Any other questions?](#still-having-issues-any-other-questions)

## Common Issues

### Couldn't create symlink

If you encounter the following error message when packing with `cpack -G DEB`:

```bash
NEXO Engine symlink couldn't be created. Required for the DEB package generator. See README's troubleshooting section.
```

To create an installer using the cpack DEB generator, our scripts try to create
a symlink to NEXO's binary. This symlink is
required for installing NEXO in the path of a DEB compatible linux distribution.

#### Solution

If you are running Windows:
- Either ignore this message, since you may not want to generate a DEB package.
- If you still want to generate a DEB package, you should enable Windows Developer Mode,
  see [Microsoft Documentaion](https://learn.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development).
  And rerun the cmake configure, build and cpack commands.

> [!WARNING]
> If you are running Windows, you may not want to generate a DEB package. It may not be supported.

If you are running another OS, it is unexpected, so please submit an issue [here](https://github.com/NexoEngine/game-engine/issues)
with the details of your OS and the error message; we will help you from there.

### CPackDeb: file utility is not available

If you encounter the following error message when packing with `cpack -G DEB`:

```bash
CPackDeb: file utility is not available.  CPACK_DEBIAN_PACKAGE_SHLIBDEPS
  and CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS options are not available.
```

The DEB generator couldn't find the `file` command.

#### Solution

You need to install the `file` command on your system. 

On Ubuntu/Debian, you can install it with the following command:

```bash
sudo apt update && sudo apt install file
```

On Windows (using chocolatey), you can install it with the following command on an elevated PowerShell:

```bash
choco install file
```

> [!WARNING]
> If you are running Windows, you may not want to generate a DEB package. It may not be supported.

## Still having issues? Any other questions?

If you are still having issues, please submit an issue [here](https://github.com/NexoEngine/game-engine/issues)
with the details of your problem. Thank you!


