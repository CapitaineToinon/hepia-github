package middlewares

import (
	"appSec/myApp/controllers"
	"appSec/myApp/env"
	"errors"
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
		if false {
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

			if _, err := jwtVerifierSetup.New().VerifyAccessToken(token); err != nil {
				log.Println(err)
				c.AbortWithStatus(http.StatusUnauthorized)
				return
			}
		}
	}
}
