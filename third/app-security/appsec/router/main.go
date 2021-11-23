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

	public := r.Group("/auth")
	{
		public.GET("/login", controllers.LoginHandler)
		public.GET("/logout", controllers.LogoutHandler)
		public.GET("/callback", controllers.AuthCodeCallbackHandler)
	}

	authorized := r.Group("/")
	authorized.Use(middlewares.AuthMiddleware())
	{
		authorized.GET("/students", controllers.GetStudentsHandler)
		authorized.GET("/teachers", controllers.GetTeachersHandler)
		authorized.POST("/students", controllers.PostStudentsHandler)
		authorized.POST("/teachers", controllers.PostTeachersHandler)
	}

	r.NoRoute(func(c *gin.Context) {
		c.File("./frontend/dist/index.html")
	})

	return r
}
