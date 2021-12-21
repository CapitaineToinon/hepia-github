package middlewares

import (
	"appSec/myApp/controllers"
	"appSec/myApp/env"
	"errors"
	"fmt"
	"log"
	"net/http"
	"strings"

	"github.com/gin-gonic/gin"
	jwtverifier "github.com/okta/okta-jwt-verifier-golang"
)

var (
	clientID         = env.Get("OKTA_CLIENT_ID")
	issuer           = env.Get("OKTA_ISSUER")
	claimsToValidate = map[string]string{
		"aud": "api://default",
		"cid": clientID,
	}
)

func AuthMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		authHeader := c.GetHeader("Authorization")

		if authHeader == "" {
			c.AbortWithStatusJSON(http.StatusBadRequest, controllers.ErrorResponse{
				Error: errors.New("missing authorization header").Error(),
			})
			return
		}

		tokenParts := strings.Split(authHeader, "Bearer ")
		token := tokenParts[1]

		jwtVerifierSetup := jwtverifier.JwtVerifier{
			Issuer:           issuer,
			ClaimsToValidate: claimsToValidate,
		}

		decoded, err := jwtVerifierSetup.New().VerifyAccessToken(token)

		if err != nil {
			log.Println(err)
			c.AbortWithStatusJSON(http.StatusUnauthorized, controllers.ErrorResponse{
				Error: errors.New("unauthorized").Error(),
			})
			return
		}

		c.Set("Jwt", decoded)
	}
}

func EmailMiddleware(email string) gin.HandlerFunc {
	return func(c *gin.Context) {
		token := c.MustGet("Jwt").(*jwtverifier.Jwt)

		if token.Claims["sub"] != email {
			message := fmt.Errorf("unauthorized, you are not %s", email).Error()
			log.Println(message)
			c.AbortWithStatusJSON(http.StatusUnauthorized, controllers.ErrorResponse{
				Error: message,
			})
			return
		}
	}
}
