# hedis-connector-mysql

## Configuration

```yaml
mysqltest:
  type: mysql
  username: MY_USERNAME
  password: MY_PASSWORD
  host: MY_LOCALHOST
  database: MY_DATABASE
  env:
    LD_LIBRARY_PATH: $LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu
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

## Command Syntax

.+

### Example

"select name from user limit 1" will return one "name" field at "user" table

### Return value

```json
{
  "columns": {
    "id": [
      "1",
      "2",
      "3",
      "4"
    ],
    "name": [
      "kewang",
      "iris",
      "NULL",
      "mitake"
    ]
  }
}
```
