package router

import (
	"appSec/myApp/controllers"
	"appSec/myApp/middlewares"

	"github.com/gin-contrib/static"
	"github.com/gin-gonic/gin"
)

func GetRouter() *gin.Engine {
	r := gin.New()
	r.Use(gin.Logger())
	r.Use(gin.Recovery())
	r.Use(middlewares.CorsMiddleware())

	r.Use(static.Serve("/", static.LocalFile("./frontend/dist", true)))

	api := r.Group("/api")
	api.Use(middlewares.AuthMiddleware())
	{
		api.GET("/students", controllers.GetStudentsHandler)
		api.GET("/teachers", controllers.GetTeachersHandler)
		api.POST("/students", controllers.PostStudentsHandler)
		api.POST("/teachers", controllers.PostTeachersHandler)
	}

	r.NoRoute(func(c *gin.Context) {
		c.File("./frontend/dist/index.html")
	})

	return r
}
