
## Setup

A password needs to be set in config/DatabaseView.json to be able to login.

The port and interface can be configured in config/Frontend.json (''address'' config).

The domain and default page can be configured in config/HttpProcessor.json (''index_path'' config).

## How to run server in terminal

$ vnxwebserver -c config/

## How to start server in background

$ ../scripts/start_server.sh

## Hwo to stop server in background

$ ../scripts/stop_server.sh

## Note on ports below 1024

If your Frontend is configured to run on port 80 then vnxwebserver needs to run as root.

Alternatively iptables can be configured to forward port 80 to 8080.
