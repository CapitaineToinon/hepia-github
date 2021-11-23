package middlewares

import (
	"appSec/myApp/auth"
	"fmt"
	"log"
	"net/http"

	"github.com/gin-gonic/gin"
	jwtverifier "github.com/okta/okta-jwt-verifier-golang"
)

func AuthMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		session, err := auth.SessionStore.Get(c.Request, "okta-hosted-login-session-store")

		if err != nil {
			log.Println(err)
			c.AbortWithStatus(http.StatusInternalServerError)
			return
		}

		raw := session.Values["access_token"]

		if raw == nil {
			log.Println(fmt.Errorf("access_token is malformed, login out just in case"))
			session.Options.MaxAge = -1
			session.Save(c.Request, c.Writer)
			c.AbortWithStatus(http.StatusUnauthorized)
			return
		}

		token := raw.(string)

		jwtVerifierSetup := jwtverifier.JwtVerifier{
			Issuer:           auth.Issuer,
			ClaimsToValidate: auth.ClaimsToValidate,
		}

		if _, err := jwtVerifierSetup.New().VerifyAccessToken(token); err != nil {
			log.Println(err)
			c.AbortWithStatus(http.StatusUnauthorized)
			return
		}
	}
}
