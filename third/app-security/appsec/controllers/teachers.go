package controllers

import (
	"appSec/myApp/database"
	"appSec/myApp/model"
	"log"
	"net/http"

	"github.com/gin-gonic/gin"
)

func GetTeachers(c *gin.Context) {
	var teachers []model.Teacher
	database.DB.Model(model.Teacher{}).Find(&teachers)

	c.JSON(http.StatusOK, gin.H{
		"data": teachers,
	})
}

func PostTeachers(c *gin.Context) {
	var registerValue model.TeacherRegister

	if err := c.BindJSON(&registerValue); err != nil {
		log.Println("Unable to bind value", err)
		return
	}

	teacher := model.Teacher{
		Lastname: registerValue.Lastname,
		Name:     registerValue.Name,
		Class:    registerValue.Class,
	}

	result := database.DB.Create(&teacher)

	if result.Error != nil {
		log.Fatal(result.Error)
		c.AbortWithStatus(http.StatusInternalServerError)
		return
	}

	c.JSON(http.StatusCreated, SuccessResponse{
		Data: teacher,
	})
}
