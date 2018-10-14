
## Setup

`$ mkdir -p data`

The port and interface can be configured in config/Frontend.json (''address'' config).

The domain and default page can be configured in config/HttpProcessor.json (''index_path'' config).

## How to run server in terminal

`$ vnxwebserver -c config/`

## How to start server in background

`$ ../scripts/start_server.sh`

## Hwo to stop server in background

`$ ../scripts/stop_server.sh`

## How to get list of modules

Visit http://localhost:8080/default/

## How to login to database

Visit http://localhost:8080/database/

Default password is "password".

## Note on ports below 1024

If your Frontend is configured to run on port 80 then vnxwebserver needs to run as root.

Alternatively iptables can be configured to forward port 80 to 8080.

## Note on connection limit

Consider increasing the maximum number of open files before starting the server:

`$ ulimit -n 100000`

This may require a system config change:

See https://www.cyberciti.biz/faq/linux-increase-the-maximum-number-of-open-files/

`$ sudo vim /etc/security/limits.conf`

Add lines for your user:

```
username         hard    nofile      1000000
username         soft    nofile      1000000
```
