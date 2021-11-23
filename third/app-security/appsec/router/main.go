package router

import (
	"appSec/myApp/controllers"

	"github.com/gin-gonic/gin"
)

func GetRouter() *gin.Engine {
	r := gin.New()
	r.Use(gin.Logger())
	r.Use(gin.Recovery())

	r.GET("/login", controllers.Login)

	r.GET("/students", controllers.GetStudents)
	r.GET("/teachers", controllers.GetTeachers)

	r.POST("/students", controllers.PostStudents)
	r.POST("/teachers", controllers.PostTeachers)

	return r
}
