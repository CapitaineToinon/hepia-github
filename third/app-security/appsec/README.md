# App sec

# Technologies

This project uses various technologies.

- The [Gin](https://github.com/gin-gonic/gin) framework for the backend (written in [Go](https://go.dev/))
- [GORM](https://gorm.io/index.html) for the database
- [Okta](https://www.okta.com/) for the authentification
- [Vue](https://v3.vuejs.org/) for the frontend (written in JavaScript)

# Docker

This project uses [docker](https://www.docker.com/) to run the project and contains various images.

## db

From [https://hub.docker.com/\_/mysql](https://hub.docker.com/_/mysql), this image is a simple mysql database and needs the following variables to be set in `.env` (see an example in `example.env`):

```
MYSQL_ROOT_PASSWORD
MYSQL_DATABASE
```

This image also has an `healthcheck` status to confirm to other images that the database is up and running. Used by the `go` docker image to not start the go application before the database is ready.

## go

From [https://hub.docker.com/\_/node/](https://hub.docker.com/_/node/) and [https://hub.docker.com/\_/golang/](https://hub.docker.com/_/golang/), this image will first build the JavaScript frontend into a static website and place the output in the `./frontend/dist/` directory.

After that, it will build the go backend. Then it will copy the generated files of the frontend into the go image in order to be served by the backend.

Finally, the image starts the application on the port `GO_PORT` but doesn't exposes it to the host as `nginx` will be used as a proxy.

This image needs the following variables to be set in `.env` (see an example in `example.env`):

```
GO_PORT
MYSQL_PORT
MYSQL_USER
MYSQL_DB
MYSQL_PASSWORD
```

## nginx

todo

## adminer

todo

# Project structure

## `./controllers/`

Contains the controllers for teachers and students endpoints for `GET` and `POST` requests. This code isn't responsable of authorisation and is thus fairy simple. For example, the following code is responsible to listing all the students from the database.

```go
func GetStudentsHandler(c *gin.Context) {
	var students []model.Student
	database.DB.Model(model.Student{}).Find(&students)

	c.JSON(http.StatusOK, SuccessResponse{
		Data: students,
	})
}
```

## `./database/`

Contains the code responsible for initialise, migrate and connect to the database, using [GORM](https://gorm.io/index.html). The code expects a MySQL connection and the following env variables need to be set accordingly in `.env` (see an example in `example.env`):

```bash
MYSQL_HOST
MYSQL_PORT
MYSQL_USER
MYSQL_DB
MYSQL_PASSWORD
```

Gorm will uses the models from the `./model/` package to create the database tables accordingly. Once created, the package exposes a `DB *gorm.DB` variable for other packages to use.

## `./docker/ and ./docker-compose.yml`

Contains all the relevant files for all the docker configuration. For more information, read the docker section.

## `./env/`

Simple wrapper package around the `os.Getenv` function that will throw exceptions when a variable isn't set or is set to an empty value instead of quitely returning an empty string like the regular package does.

## `./frontend/`

JavaScript [Vue](https://v3.vuejs.org/) project for the frontend.

## `./middlewares/`

Contains the 2 middlewares for this project.

### `auth.go`

Middleware responsible for the authentification. It uses [`okta-jwt-verifier-golang`](https://github.com/okta/okta-jwt-verifier-golang) to verify the token and expects a JWT token in the Authorization header of the requests.

### `cors.go`

This simple middleware uses [`rs/cors`](https://github.com/rs/cors) to allow cors requests to the API.

## `./model/`

Contains all the models used in the application. Those models are also used by the `database` package to create the tables accordingly.

## `./router/`

Package responsible to setup the gin router.
