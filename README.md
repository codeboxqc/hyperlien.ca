# hyperlien.ca
Free video Downloader


Hyperlien URL Dropping Application
Description
Hyperlien is a simple application that allows users to drag and drop URLs (such as YouTube links) directly into the program. It supports the http:// and https:// URL schemes, and processes them accordingly.

The application allows for easy handling of URLs, providing the user with a simple and intuitive interface.

Features
Drag and drop URLs from the browser directly into the application.
Automatically detects valid web links (URLs).
Displays the dropped URL in an edit box.
Optionally adds the URL to a listbox for further use.
Prerequisites
This application requires the following tools:

1. yt-dlp  
yt-dlp is a powerful open-source tool to download videos from YouTube and other websites. It is a fork of youtube-dl and includes many extra features, fixes, and performance improvements.

2. ariac2 (Freeware for Video Downloading)
ariac2 is another video downloader tool that is integrated with the application for added functionality.

Installation
 
Run Hyperlien Application:

Double-click on the Hyperlien.exe file (Windows) or run the compiled binary (Linux/macOS).
The main window will appear with an editable text box and a list box.
Drag and Drop URLs:

Drag any URL from your browser (e.g., YouTube, Vimeo, etc.) and drop it onto the application window.
If the dropped link is a valid URL, it will be displayed in the text box. Optionally, it will be added to the list box for further actions.
Download Video Using yt-dlp:

When a valid URL is dropped, you can invoke yt-dlp to download the video by running a shell command or integrating it into the program (you'll need to script this part).
Use ariac2 for Extra Features:

Similar to yt-dlp, ariac2 can be used for video downloading or other supported features. Ensure both yt-dlp and ariac2 are in the same directory as your program for smooth integration.
Troubleshooting
Application does not accept URL drag-and-drop:
Make sure your window is set up to accept drag-and-drop files.
Ensure the program is running with sufficient permissions to handle drag-and-drop events.
yt-dlp or ariac2 not working:
Check that the respective executables are located in the same directory as the Hyperlien application.
Ensure you have downloaded the latest version of both tools.
License
This application and associated libraries (yt-dlp, ariac2) are provided "as is". No warranties are expressed or implied.

Hyperlien: MIT License

yt-dlp: MIT License

ariac2: Please refer to its own licensing information.

Contact Class@nutz.club
