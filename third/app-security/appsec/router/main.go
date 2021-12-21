package router

import (
	"appSec/myApp/controllers"
	"appSec/myApp/middlewares"

	"github.com/gin-contrib/static"
	"github.com/gin-gonic/gin"
)

var postUser = "test@gmail.com"
var deleteUser = "johndoe@gmail.com"

func GetRouter() *gin.Engine {
	r := gin.New()
	r.Use(gin.Logger())
	r.Use(gin.Recovery())
	r.Use(middlewares.CorsMiddleware())

	r.Use(static.Serve("/", static.LocalFile("./frontend/dist", true)))

	api := r.Group("/api")
	api.GET("/students", controllers.GetStudentsHandler)
	api.GET("/teachers", controllers.GetTeachersHandler)

	api.Use(middlewares.AuthMiddleware())
	{
		api.POST("/students", middlewares.EmailMiddleware(postUser), controllers.PostStudentsHandler)
		api.POST("/teachers", middlewares.EmailMiddleware(postUser), controllers.PostTeachersHandler)
		api.DELETE("/students", middlewares.EmailMiddleware(deleteUser), controllers.DeleteStudent)
		api.DELETE("/teachers", middlewares.EmailMiddleware(deleteUser), controllers.DeleteTeacher)
	}

	r.NoRoute(func(c *gin.Context) {
		c.File("./frontend/dist/index.html")
	})

	return r
}
