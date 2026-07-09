# CviConverter
A utility for converting National Instruments LabWindows/CVI `.uir` files into JSON for using in modern web applications.
---
## About
The project was developed to convert NI LabWindows .uir files into .json files for displaying the electrical panel which initially were created with LabWindows in web-pages. The application accepts a name of an .uir file as an argument and reads its properties using NI libs. After that it also reads some settings from a database. Next the application creates .json which describes the panel's structure and contains all the properties and settings which were read.
---
## Features
• Can use a database as a sourse of additional parameters or can work without it as well
• Has the script for a batch conversion (optional)
---
## Architecture
```text
			C#
			│
			│ P/Invoke
			▼
	C++ Wrapper DLL
			│
			│
			▼
National Instruments CVI Runtime
			│
			▼
		UIR file
			│
			▼
		JSON output
```
---   
## Technologies
• C#
• .NET
• C++
• P/Invoke
• DllImport
• National Instruments LabWindows/CVI
• Dependency Injection
• JSON
• Entity Framework
• Serilog
---
## Build Notes

The project depends on proprietary National Instruments libraries and the LabWindows/CVI Runtime.Because these components cannot be redistributed, the project cannot be built or executed out of the box without the required NI dependencies.The source code is provided to demonstrate the architecture and implementation of the converter.
---
## Motivation
This project was created while migrating a legacy desktop application based on National Instruments LabWindows/CVI. The goal was to extract UI resources automatically and convert them into a format suitable for further processing by modern applications.