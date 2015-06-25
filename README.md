# hedis-connector-mysql

## Configuration

```yaml
mysqltest:
  type: mysql
  username: MY_USERNAME
  password: MY_PASSWORD
  host: MY_LOCALHOST
  database: MY_DATABASE
```

## Requirement

* Debian / Ubuntu

```sh
sudo apt-get install libmysqlclient-dev
```

## Build & Install

```sh
make
sudo make install
```

## Run with Hedis

### Enviroment Variables

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu
```

## Command Syntax

.+

### Example

"select name from user limit 1" will return one "name" field at "user" table
