package controllers

import (
	"appSec/myApp/database"
	"appSec/myApp/model"
	"log"
	"net/http"

	"github.com/google/uuid"

	"github.com/gin-gonic/gin"
)

func GetStudents(c *gin.Context) {
	var students []model.Student
	database.DB.Model(model.Student{}).Find(&students)

	c.JSON(http.StatusOK, gin.H{
		"data": students,
	})
}

func PostStudents(c *gin.Context) {
	var student model.Student

	if err := c.Bind(&student); err != nil {
		log.Println("Unable to bind value", err)
		c.AbortWithStatusJSON(http.StatusBadRequest, gin.H{
			"message": err.Error(),
		})
		return
	}

	student.UUID = uuid.New()
	result := database.DB.Create(&student)

	if result.Error != nil {
		log.Fatal(result.Error)
		c.AbortWithStatus(http.StatusInternalServerError)
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message": "ok",
	})
}
