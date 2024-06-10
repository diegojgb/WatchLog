# WatchLog

<img src="https://github.com/diegojgb/WatchLog/blob/main/screenshots/screenshot1.png">
<img src="https://github.com/diegojgb/WatchLog/blob/main/screenshots/screenshot2.png">
<img src="https://github.com/diegojgb/WatchLog/blob/main/screenshots/screenshot3.png">
<img src="https://github.com/diegojgb/WatchLog/blob/main/screenshots/screenshot4.png">

## What is it?

WatchLog is a simple tool that lets you monitor any log/text file, and trigger notifications and/or sound alerts upon match of user-defined regex patterns. 

Any time there is a new line added to the log, it is tested agaisn't all regex patterns specified for that file. Each one has its own configuration on how it notifies the user when there's a match. You can monitor multiple log files and have as many regex patterns as you want.

- [Installation](#installation)
- [Usage](#usage)
- [Hidden settings](#hidden-settings-in-datajson)
- [data.json structure](#datajson-structure)
    - [Monitor](#monitor)
    - [Notifier](#notifier)
    - [data.json example](#datajson-example)
- [Source dependencies](#source-dependencies)
    - [JSON for Modern C++](#json-for-modern-c)
    - [WinToast](#wintoast)
- [Limitations](#limitations)

## Installation

- You can download the latest release from [here](https://github.com/diegojgb/WatchLog/releases/latest "https://github.com/diegojgb/WatchLog/releases/latest")
- It is a standalone executable. It reads its configuration from a single "data.json" file.

## Usage

1. Download the latest version from [here](https://github.com/diegojgb/WatchLog/releases/latest "https://github.com/diegojgb/WatchLog/releases/latest"), unzip the package, and run the application.
2. Create a "monitor" (click on "Add monitor"), and set a name and file you want to watch.
3. On the newly created monitor, click on "Add notification", there you can define the regex pattern of your choice, and configure the notification you want to get.

4. You're presented with the following options:
    - **Name:** name associated with your notification.
    - **Regex:** the regex pattern to be searched for.
    - **Title:** title displayed in the toast notification. It's set to "Match found in \${name}" by default. ${name} is a placeholder for the containing Monitor's name.
    - **Description:** description displayed in the toast notification . It's set to "For regex: \${regex}" by default. ${regex} is a placeholder for the regex pattern.
    - **Image:** image to be displayed inside the toast notification. 60x60 is the recommended image size. Comes with a default file, included in the package you downloaded.
    - **Sound:** sound to be played either with or without a toast notification.
    - **Duration:** toast notification display time. Can be either "System", "Short" or "Long". Durations for each setting are defined within the Windows configuration.
    - **Sticky:** when enabled, toast notifications are displayed until dismissed (they don't go away).

5. When finished, click on "Add". After that, you can enable the Toast and/or Sound option(s).
6. Monitor as many text files as you want, and have a separate set of regex/notifications for each one (as many as you want).
7. When a match is found, you will receive a Windows toast notification and/or a sound alert. The sound can be the Windows default or a custom user-defined sound.

## Hidden settings in data.json

There are a few hidden settings that can only be modified directly through the data.json.

- `manyPerUpdate` (monitor): When disabled, if two potential matches come in a single file update, only the first one will be found. It's set to `false` by default.
- `staticDefaultImage` (monitor): this sets a static "default of default" image file path. It overrides the application-wide default for this specific monitor. It can be a relative path.
- `winFileMode` (root): If set to "WinApi" or "Mixed", an experimental mode will be enabled that leverages the Windows API to provide immediate updates when any monitored/utilized file is created or deleted. It's set to "Manual" by default. 

## data.json structure

The entire application is built based on the data.json file. It is a JSON file that contains an `array` of "monitors", each one with its own `array` of "Notifiers":

- **Monitor:** contains the path of a file to watch and a list of notifiers for that file.
- **Notifier:** represents a regex pattern to search within new lines of the corresponding log file, triggering a notification when a match is found.

### Root

At its root, the data.json contains the following properties:

| Property | Type | Default | Description |
|--|--|--|--|
| `monitors` (required) | array | None | Array that contains all the defined monitors. |
| `winFileMode` | string | `"Manual"` | Sets the way the app checks for the deletion/creation of monitored/utilized files. It can be either "Manual" (checks every 10 seconds), "WinApi" (utilizes the Windows API -ReadDirectoryChangesW-) or "Mixed", utilizes both methods at the same time. |


### Monitor

Monitors consist of the following properties:

| Property | Type | Default | Description |
|--|--|--|--|
| `name` (required) | string | None | Monitor's name. Can be any string. |
| `filePath` (required) | string | None | Absolute path of the file to be monitored. You can use either "/" or "\\\\" in the address. |
| `enabled` | boolean | `true` | Initial state of the monitor (on/off). |
| `manyPerUpdate` | boolean | `false` | When disabled, if two potential matches come in a single file update, only the first one will be found. |
| `staticDefaultImage` | string | None | Overrides the application-wide default for this specific monitor. It must be a jpg/jpeg/png file. |
| `defaultImage` | string | `"./assets/information.png"` | Default image used for Notifiers that do not specify an image. 60x60 is the recommended image size. It must be a jpg/jpeg/png file. |
| `defaultSound` | string | `"%SystemDrive%/Windows/Media/Windows Notify System Generic.wav"` | Default sound used for Notifiers that do not specify a sound file. It must be a wav file. |
| `notifiers` (required) | boolean | None | List of notifiers. Refer to the Notifiers section for more information. |

#### Full monitor example:

A Monitor with all of its properties explicity set would look like this:

```json
{
    "defaultImage": "C:/example.png",
    "defaultSound": "C:/example.wav",
    "enabled": true,
    "filePath": "C:/example.log",
    "name": "Monitor example",
    "manyPerUpdate": false,
    "notifiers": [
        "..."
    ],
    "staticDefaultImage": "./assets/example.png"
}
```

#### Minimal monitor example:

A Monitor using all of its default values on non-required properties would look like this:

```json
{
    "filePath": "C:/example.log",
    "name": "Monitor example",
    "notifiers": [
        "..."
    ]
}
```

### Notifier

Each monitor includes one or more notifiers, each containing a specific regex pattern and a set of configurations for notifying the user in case of a match.
    
| Property | Type | Default | Description |
|--|--|--|--|
| `name` (required) | string | None | Notifier's name. Usually describes under what conditions it triggers a notification. |
| `pattern` (required) | string | None | The regex pattern to be searched for. |
| `title` | string | `"Match found in ${name}"` | Title displayed in the toast notification. ${name} is a placeholder for the containing Monitor's name. |
| `desc` | string | `"For regex: ${regex}"` | Description displayed in the toast notification. ${regex} is a placeholder for the regex pattern. |
| `soundFile` | string | Monitor's `defaultSound` | Sound to be played either with or without a toast notification. |
| `image` | string | Monitor's `defaultImage` | Path to the image to be displayed inside the toast notification. 60x60 is the recommended image size. |
| `duration` | string | `"System"` | Toast notification display time. Can be either "System", "Short" or "Long". Durations for each setting are defined within the Windows configuration. |
| `toast` | boolean | `true` | Sets wether a toast notification will be shown when a match is found. |
| `sound` | boolean | `true` | Sets wether a sound alert will be played when a match is found. |
| `sticky` | boolean | `false` | When enabled, toast notifications are displayed until dismissed (they don't go away). |

#### Full Notifier example

A Notifier with all of its properties explicity set would look like this:

```json
{
    "name": "When a phone number is added to the log.",
    "pattern": "^(\\+\\d{1,2}\\s)?\\(?\\d{3}\\)?[\\s.-]\\d{3}[\\s.-]\\d{4}$",
    "title": "Found a phone number!",
    "desc": "Should we call?",
    "soundFile": "C:/example.wav",
    "image": "C:/example.png",
    "duration": "Short",
    "toast": true,
    "sound": true,
    "sticky": true,
}
```

#### Minimal notifier example

A Notifier using all of its default values on non-required properties would look like this:

```json
{
    "name": "When a phone number is added to the log.",
    "pattern": "^(\\+\\d{1,2}\\s)?\\(?\\d{3}\\)?[\\s.-]\\d{3}[\\s.-]\\d{4}$",
}
```

### Full data.json example

```json
{
    "monitors": [
        {
            "enabled": true,
            "filePath": "C:/example.log",
            "name": "Monitor example",
            "manyPerUpdate": false,
            "notifiers": [
                {
                    "name": "When a phone number is added to the log.",
                    "pattern": "^(\\+\\d{1,2}\\s)?\\(?\\d{3}\\)?[\\s.-]\\d{3}[\\s.-]\\d{4}$",
                    "title": "Found a phone number!",
                    "desc": "Should we call?",
                    "soundFile": "C:/example.wav",
                    "image": "C:/example.png",
                    "duration": "Short",
                    "toast": true,
                    "sound": true,
                    "sticky": true,
                },
                {
                    "name": "When a phone number is added to the log.",
                    "pattern": "^(\\+\\d{1,2}\\s)?\\(?\\d{3}\\)?[\\s.-]\\d{3}[\\s.-]\\d{4}$",
                },
                "..."
            ],
            "staticDefaultImage": "./assets/fg-faceplate.png"
        },
        {
            "filePath": "C:/another_example.log",
            "name": "Another monitor example",
            "notifiers": [
                {
                        "name": "When a wild Hello World appears.",
                        "pattern": "Hello World",
                },
                "..."
            ]
        },
        "..."
    ],
    "winFileMode": "Mixed"
}
```

## Source dependencies

If you want to work on the application and build it by yourself using Qt Creator, Visual Studio, or your preferred method, you need to include these 2 dependencies on your project:

- [JSON for Modern C++](https://github.com/nlohmann/json "https://github.com/nlohmann/json") - by [mohabouje](https://github.com/mohabouje "https://github.com/mohabouje").
- [Fork of WinToast](https://github.com/diegojgb/WinToast "https://github.com/diegojgb/WinToast") - by [nlohmann](https://github.com/nlohmann "https://github.com/nlohmann").

### JSON for Modern C++

Easiest way to add it is to simply download the single-file release (json.hpp), and include it on the project as src/nlohmann/json.hpp.

### WinToast

The CMakeLists.txt already includes WinToast as a subdirectory, so you just have to clone WinToast's repository and place it inside the root folder, along with WatchLog's CMakeLists.txt.

This app uses a modified version of WinToast, that allows the removal of previously defined "Actions" from notification templates.

## Limitations

- Spamming many Windows toasts in a short period of time can lead to unexpected behavior, that may continue until the application is restarted. Just triggering two toast notifications within milliseconds from each other is enough to break it, that's the reason behind the ["manyPerUpdate"](#monitor) property, which, if set to false, prevents the triggering of multiple notifications on a single file update.