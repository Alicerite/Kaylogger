# Kaylogger
A Substance use logging program with comparability with AnodyneWiki's Discord bot and CSV files
**For usage when placed inside of a folder:**

    ./kaylog


**To install system wide on linux:**

    sudo mv ./kaylog /usr/local/bin/kaylog
    sudo chmod +x /usr/local/bin/kaylog

## Options
| Option | Shorthand | Action |
| :--- | :--- | :--- |
| `--csv <file>` | `-c <file>` | Designates the CSV file path to be used |
| `--webhook <url>` | `-k <url>` | Connects to a Discord webhook URL for use with a bot |
| `--user <name>` | `-u <name>` | Specifies the name of the User |
| `--substance <name>` | `-s <name>` | Specifies the substance name |
| `--salt <salt>` | `-a <salt>` | Specifies the salt form (if known) |
| `--dosage <amount>` | `-d <amount>` | Sets the dosage value |
| `--route <roa>` | `-r <roa>` | Defines the route of administration |
| `--site <site>` | `-i <site>` | Defines the specific site of administration |
| `--note <note>` | `-n <note>` | Adds a note to the entry if noteworthy |
| `--help` | `-h` | Displays this help text |

**Example:**

    ./kaylog -c logs.csv -k "https://discord.com/api/webhooks/ID/TOKEN" -u "LainaPony" -s "Caffeine" -a "Anhydrous" -d "200mg" -r "Oral" -i "oral-cavity" -n "Taken with breakfast :3"

Created by Kayrite
Documented by LainaPony
