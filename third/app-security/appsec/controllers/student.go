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

func GetStudentsHandler(c *gin.Context) {
	var students []model.Student
	database.DB.Model(model.Student{}).Find(&students)

	c.JSON(http.StatusOK, SuccessResponse{
		Data: students,
	})
}

func PostStudentsHandler(c *gin.Context) {
	var registerValue model.StudentRegister

	if err := c.Bind(&registerValue); err != nil {
		log.Println("Unable to bind value", err)
		c.AbortWithStatusJSON(http.StatusBadRequest, ErrorResponse{
			Error: err.Error(),
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
		c.AbortWithStatusJSON(http.StatusInternalServerError, ErrorResponse{
			Error: errors.New("internal server error").Error(),
		})
		return
	}

	c.JSON(http.StatusCreated, SuccessResponse{
		Data: student,
	})
}

func DeleteStudent(c *gin.Context) {
	var deleteValue model.StudentDelete

	if err := c.Bind(&deleteValue); err != nil {
		log.Println("Unable to bind value", err)
		c.AbortWithStatusJSON(http.StatusBadRequest, ErrorResponse{
			Error: err.Error(),
		})
		return
	}

	result := database.DB.Unscoped().Delete(&model.Student{}, deleteValue.ID)

	if result.Error != nil {
		if errors.Is(result.Error, gorm.ErrRecordNotFound) {
			c.AbortWithStatusJSON(http.StatusNotFound, ErrorResponse{
				Error: errors.New("student not found").Error(),
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
