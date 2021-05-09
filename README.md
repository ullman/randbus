# randbus

## Description

Randbus uses a simple protocol for receiving sensor data over UART for scraping
by prometheus.

## Dependencies

General:
```bash
sudo apt install build-essential automake zlib1g-dev libmicrohttpd-dev
```
and
```bash
libprom-dev
libpromhttp-dev
```
available [here](https://github.com/digitalocean/prometheus-client-c)


## Compiling

```bash
$ ./autogen.sh
$ ./configure
$ make
```

## Usage

```bash
randbus -i /dev/ttyX
```


```text
-i [Serial device]
-p [Port for Prometheus exporter]
```
## Prometheus configuration for exporting with randbus

Add the following in your Prometheus config file, `prometheus.yml`, under the section `scrape_configs:`
Adjust the port to match velog. Adjust ip if scraping from another machine.

```text
  - job_name: randbus
    static_configs:
      - targets: ['localhost:9009']
```

After restarting Prometheus, the velog data is available in the format:

`randbus{label="SENSOR-NAME"}`

## Licence

GPL Version 3

See `COPYING` for details
