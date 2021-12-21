package controllers

import (
	"appSec/myApp/database"
	"appSec/myApp/model"
	"errors"
	"log"
	"net/http"

	"github.com/gin-gonic/gin"
	"gorm.io/gorm"
)

func GetTeachersHandler(c *gin.Context) {
	var teachers []model.Teacher
	database.DB.Model(model.Teacher{}).Find(&teachers)

	c.JSON(http.StatusOK, SuccessResponse{
		Data: teachers,
	})
}

func PostTeachersHandler(c *gin.Context) {
	var registerValue model.TeacherRegister

	if err := c.BindJSON(&registerValue); err != nil {
		log.Println("Unable to bind value", err)
		c.AbortWithStatusJSON(http.StatusBadRequest, ErrorResponse{
			Error: err.Error(),
		})
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
		c.AbortWithStatusJSON(http.StatusInternalServerError, ErrorResponse{
			Error: errors.New("internal server error").Error(),
		})
		return
	}

	c.JSON(http.StatusCreated, SuccessResponse{
		Data: teacher,
	})
}

func DeleteTeacher(c *gin.Context) {
	var deleteValue model.TeacherDelete

	if err := c.Bind(&deleteValue); err != nil {
		log.Println("Unable to bind value", err)
		c.AbortWithStatusJSON(http.StatusBadRequest, ErrorResponse{
			Error: err.Error(),
		})
		return
	}

	result := database.DB.Unscoped().Delete(&model.Teacher{}, deleteValue.ID)

	if result.Error != nil {
		if errors.Is(result.Error, gorm.ErrRecordNotFound) {
			c.AbortWithStatusJSON(http.StatusNotFound, ErrorResponse{
				Error: errors.New("teacher not found").Error(),
			})
		}

		log.Fatal(result.Error)
		c.AbortWithStatusJSON(http.StatusInternalServerError, ErrorResponse{
			Error: errors.New("internal server error").Error(),
		})
		return
	}

	c.JSON(http.StatusOK, SuccessResponse{
		Data: "Deleted",
	})
}
