# WatchLog

<img src="https://github.com/diegojgb/xPaste/blob/main/screenshot.png" width="828" height="597">

## What is it?

WatchLog is a simple tool that lets you monitor any log file, and trigger notifications and/or sound alerts upon match of user-defined regex patterns. 

Any time there is a new line added to the log, it is tested agaisn't all regex patterns specified for that file. Each one has its own configuration on how it notifies the user when there's a match. You can monitor multiple log files and have as many regex patterns as you want.

## Installation

- You can download the latest release from [here](https://github.com/diegojgb/xPaste/releases/latest "https://github.com/diegojgb/xPaste/releases/latest")
- It is a standalone executable. It reads its configuration from a single "data.json" file. Refer to the section below for configuring your data.json

## Usage

You need to create a data.json file that holds all the information needed for the application to run. This file must reside in the same folder as the executable. For more information refer to the [configuring data.json](#configuring-data.json) section.

### data.json example

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
                // ...
            ]
        },
        {
            "filePath": "C:/another_example.log",
            "name": "Another monitor example",
            "notifiers": [
                {
                        "name": "When a wild Hello World appears.",
                        "pattern": "Hello World",
                },
                // ...
            ]
        }
    ]
}
```

## Configuring data.json

The entire application is built based on the data.json file. It is a JSON file that contains an `array` of "Monitors", each one with its own `array` of "Notifiers":

- **Monitor:** contains the path of a file to watch and a list of notifiers for that file.
- **Notifier:** represents a regex pattern to search within new lines of the corresponding log file, triggering a notification when a match is found.

### Monitor

Monitors consist of the following properties:

| Property | Type | Default | Description |
|--|--|--|--|
| `name` (required) | string | None | Monitor's name. Can be any string |
| `filePath` (required) | string | None | Absolute path of the file to be monitored. You can use either "/" or "\\\\" in the address |
| `enabled` | boolean | `true` | Initial state of the monitor (on/off) |
| `manyPerUpdate` | boolean | `true` | When disabled, if two potential matches come in a single file update, only the first one will be found |
| `notifiers` (required) | boolean | None | List of notifiers. Refer to the Notifiers section for more information |

#### Full monitor example:

A Monitor with all of its properties explicity set would look like this:

```json
{
    "enabled": true,
    "filePath": "C:/example.log",
    "name": "Monitor example",
    "manyPerUpdate": false,
    "notifiers": [
        // ...
    ]
}
```

#### Minimal monitor example:

A Monitor using all of its default values on non-required properties would look like this:

```json
{
    "filePath": "C:/example.log",
    "name": "Monitor example",
    "notifiers": [
        // ...
    ]
}
```

### Notifier

Each monitor includes one or more notifiers, each containing a specific regex pattern and a set of configurations for notifying the user in case of a match.
    
| Property | Type | Default | Description |
|--|--|--|--|
| `name` (required) | string | None | Notifier's name. Usually describes under what conditions the it triggers a notification |
| `pattern` (required) | string | None | The regex pattern to be searched for |
| `title` | string | `"Match found in ${name}"` | Title displayed in the toast notification. ${name} is a placeholder for the containing Monitor's name |
| `desc` | string | `"For regex: ${regex}"` | Description displayed in the toast notification. ${regex} is a placeholder for the regex pattern. |
| `soundFile` | string | `"%SystemDrive%/Windows/Media/Windows Notify System Generic.wav"` | Sound to be played either with or without a toast notification |
| `image` | string | `"./assets/information.png"` | Path to the image to be displayed inside the toast notification |
| `duration` | string | `"System"` | Toast notification display time. Can be either "System", "Short" or "Long". Durations for each setting are defined within the Windows configuration |
| `toast` | boolean | `true` | Sets wether a toast notification will show or not when a match is found. |
| `sound` | boolean | `true` | Sets wether a sound alert will be played when a match is found. |
| `sticky` | boolean | `true` | When enabled, toast notifications are displayed until dismissed (they don't go away) |

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
