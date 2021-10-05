package router

import (
	"appSec/myApp/controllers"

	"github.com/gin-gonic/gin"
)

func GetRouter() *gin.Engine {
	r := gin.Default()

	r.GET("/students", controllers.GetStudents)
	r.GET("/teachers", controllers.GetTeachers)

	r.POST("/students", controllers.PostStudents)
	r.POST("/teachers", controllers.PostTeachers)

	return r
}
