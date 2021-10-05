package controllers

import (
	"appSec/myApp/database"
	"appSec/myApp/model"
	"log"
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/google/uuid"
)

func GetTeachers(c *gin.Context) {
	var teachers []model.Teacher
	database.DB.Model(model.Teacher{}).Find(&teachers)

	c.JSON(http.StatusOK, gin.H{
		"data": teachers,
	})
}

func PostTeachers(c *gin.Context) {
	var teacher model.Teacher

	if err := c.Bind(&teacher); err != nil {
		log.Println("Unable to bind value", err)
		c.AbortWithStatusJSON(http.StatusBadRequest, gin.H{
			"message": err.Error(),
		})
		return
	}

	teacher.UUID = uuid.New()
	result := database.DB.Create(&teacher)

	if result.Error != nil {
		log.Fatal(result.Error)
		c.AbortWithStatus(http.StatusInternalServerError)
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message": "ok",
	})
}
