package controllers

import (
	"appSec/myApp/database"
	"appSec/myApp/model"
	"log"
	"net/http"

	"github.com/gin-gonic/gin"
)

func GetStudents(c *gin.Context) {
	if err := verify(c); err != nil {
		c.AbortWithStatus(http.StatusUnauthorized)
		return
	}

	var students []model.Student
	database.DB.Model(model.Student{}).Find(&students)

	c.JSON(http.StatusOK, gin.H{
		"data": students,
	})
}

func PostStudents(c *gin.Context) {
	var registerValue model.StudentRegister

	if err := c.Bind(&registerValue); err != nil {
		log.Println("Unable to bind value", err)
		c.AbortWithStatusJSON(http.StatusBadRequest, gin.H{
			"message": err.Error(),
		})
		return
	}

	student := model.Student{
		Lastname: registerValue.Lastname,
		Name:     registerValue.Name,
		Filiere:  registerValue.Filiere,
	}

	result := database.DB.Create(&student)

	if result.Error != nil {
		log.Fatal(result.Error)
		c.AbortWithStatus(http.StatusInternalServerError)
		return
	}

	c.JSON(http.StatusCreated, SuccessResponse{
		Data: student,
	})
}
