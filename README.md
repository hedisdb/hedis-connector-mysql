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

* Debain / Ubuntu

```sh
sudo apt-get install libmysqlclient-dev
```

## Environment Variables

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu
```
